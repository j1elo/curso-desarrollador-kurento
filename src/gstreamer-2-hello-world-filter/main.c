// <#file>

#include <gst/gst.h>

#include <stdbool.h>

// ----------------------------------------------------------------------------

GMainLoop *loop;

void intHandler(int unused) {
    g_print ("[intHandler] Quit main loop\n");
    g_main_loop_quit (loop);
}

// ----------------------------------------------------------------------------

gboolean apply_filter_ms (GstElement *video_filter)
{
  static float value_inc = 0.1;
  static float value = 0.0;

  g_object_set (video_filter, "saturation", value, NULL);

  value += value_inc;

  if (value > 1.5) {
      value_inc = -value_inc;
  }

  if (value <= 0.1) {
      value_inc = -value_inc;
  }

  return TRUE;
}

gboolean apply_filter_s (GstElement *video_filter)
{
  static gboolean active = FALSE;

  if (active) {
    g_object_set (video_filter, "saturation", 1.0, NULL);
  }
  else {
    g_object_set (video_filter, "saturation", 0.0, NULL);
  }

  active = !active;

  return TRUE;
}

// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  // Initialize GStreamer
  gst_init (&argc, &argv);



  /* GStreamer pipeline setup
   * ========================
   *
   * This includes setting up a bus watcher, to show messages that get posted.
   */

  GstElement *pipeline = gst_pipeline_new ("gstreamer-hello-world");



  /* GStreamer element setup
   * =======================
   */

  GstElement *video_source = gst_element_factory_make ("videotestsrc", "video_source");
  GstElement *video_filter = gst_element_factory_make ("videobalance", "video_filter");
  GstElement *video_sink = gst_element_factory_make ("autovideosink", "video_sink");

  if (!pipeline || !video_source || !video_sink || !video_filter) {
    g_print ("[main] ERROR: Not all elements could be created\n");
    return -1;
  }

  // Add all elements to the pipeline
  gst_bin_add_many (GST_BIN (pipeline), video_source, video_sink, video_filter,
                    NULL);

  /* Link elements together. Reference:
   *
   * gst-launch-1.0 \
   *     videotestsrc ! autovideosink
   */
  gst_element_link (video_source, video_filter);
  gst_element_link (video_filter, video_sink);

  // Set PLAYING state to pipeline and all its elements
  gst_element_set_state (pipeline, GST_STATE_PLAYING);



  /* Configure timed events
   * ======================
   */

  g_timeout_add (100, (GSourceFunc)apply_filter_ms, video_filter);
  //g_timeout_add_seconds (3, (GSourceFunc)apply_filter_s, video_filter);



  /* Application main loop
   * =====================
   */

  const gchar *dot_name = "pipeline";
  g_print ("[main] Please export this variable: GST_DEBUG_DUMP_DOT_DIR=$PWD\n");
  g_print ("[main] Generate GStreamer DOT file: %s\n", dot_name);

  // Env var 'GST_DEBUG_DUMP_DOT_DIR' must be set
  GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline), GST_DEBUG_GRAPH_SHOW_ALL, dot_name);

  g_print ("[main] Run the main loop...\n");

  signal(SIGINT, intHandler);

  // Initialize the application's main loop, that handles all events
  loop = g_main_loop_new (NULL, FALSE);

  g_main_loop_run (loop);  // Blocks execution



  /* Cleanup
   * =======
   */
  g_print ("[main] Main loop finished!\n");

  g_print ("[main] Stop playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("[main] Delete the pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  g_main_loop_unref (loop);

  return 0;
}

// ----------------------------------------------------------------------------
