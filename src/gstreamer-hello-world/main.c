// <#file>

#include <gst/gst.h>

#include <signal.h>
#include <stdbool.h>
//#include <stdio.h>
//#include <string.h>

// ----------------------------------------------------------------------------

static volatile bool keepRunning = true;

void intHandler(int unused) {
    keepRunning = false;
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
  GstElement *video_sink = gst_element_factory_make ("autovideosink", "video_sink");

  if (!pipeline || !video_source || !video_sink) {
    g_print ("[main] ERROR: Not all elements could be created\n");
    return -1;
  }

  // Add all elements to the pipeline
  gst_bin_add_many (GST_BIN (pipeline), video_source, video_sink,
                    NULL);

  /* Link elements together. Reference:
   *
   * gst-launch-1.0 \
   *     videotestsrc ! autovideosink
   */
  gst_element_link (video_source, video_sink);

  // Set PLAYING state to pipeline and all its elements
  gst_element_set_state (pipeline, GST_STATE_PLAYING);



  /* Application main loop
   * =====================
   */

  const gchar *dot_name = "pipeline";
  g_print ("[main] Export GStreamer DOT file: %s\n", dot_name);

  // Env var 'GST_DEBUG_DUMP_DOT_DIR' must be set
  GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline), GST_DEBUG_GRAPH_SHOW_ALL, dot_name);


  g_print ("[main] Run the main loop...\n");
  signal(SIGINT, intHandler);
  while (keepRunning);



  /* Cleanup
   * =======
   */
  g_print ("[main] Main loop finished!\n");

  g_print ("[main] Stop playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("[main] Delete the pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));

  return 0;
}

// ----------------------------------------------------------------------------
