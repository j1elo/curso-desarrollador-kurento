// <#file>

#include <gst/gst.h>

//#include <stdio.h>
//#include <string.h>

// ----------------------------------------------------------------------------

static char *video_uri = "https://www.freedesktop.org/software/gstreamer-sdk/data/media/sintel_trailer-480p.webm";

// ----------------------------------------------------------------------------

typedef struct {
  GstElement *audio_sink;
  GstElement *video_sink;
} demux_sinks_t;

static void
on_demux_pad_added (GstElement *demux, GstPad *new_pad,
                    demux_sinks_t *demux_sinks)
{
  // Print debug info

  g_print ("[on_demux_pad_added] A new pad '%s' was created by element '%s'\n",
           GST_PAD_NAME (new_pad), GST_ELEMENT_NAME (demux));

  GstCaps *new_caps = gst_pad_get_current_caps (new_pad);

  gchar *new_caps_str = gst_caps_to_string (new_caps);
  g_print ("[on_demux_pad_added] New pad's caps are %s\n", new_caps_str);
  g_free (new_caps_str);



  // Connect to either audio or video decoder

  GstElement *sink = NULL;
  GstStructure *new_caps_struct = gst_caps_get_structure (new_caps, 0);
  const gchar *new_caps_type = gst_structure_get_name (new_caps_struct);

  if (g_str_has_prefix (new_caps_type, "audio/")) {
    g_print ("[on_demux_pad_added] It's an audio pad\n");
    sink = demux_sinks->audio_sink;
  }
  else if (g_str_has_prefix (new_caps_type, "video/")) {
    g_print ("[on_demux_pad_added] It's a video pad\n");
    sink = demux_sinks->video_sink;
  }

  // Get the decoder sink pad
  GstPad *sink_pad = gst_element_get_static_pad (sink, "sink");

  // Connect the new demux pad to the decoder pad
  gst_pad_link (new_pad, sink_pad);

  gst_caps_unref (new_caps);
  gst_object_unref (sink_pad);
}

// ----------------------------------------------------------------------------

static gboolean
on_bus_message (GstBus *bus, GstMessage *message, GMainLoop *loop)
{
  gboolean quit = FALSE;
  GError *err = NULL;
  gchar *debug = NULL;

  switch (GST_MESSAGE_TYPE (message)) {
  case GST_MESSAGE_ERROR:
    quit = TRUE;

    gst_message_parse_error (message, &err, &debug);
    g_printerr ("[on_bus_message] ERROR %d: %s\n", err->code, (err->message ? err->message : "(None)"));
    g_printerr ("[on_bus_message] Debug info: %s\n", (debug ? debug : "(None)"));
    g_error_free (err);
    g_free (debug);

    break;
  case GST_MESSAGE_WARNING:
    quit = FALSE;

    gst_message_parse_warning (message, &err, &debug);
    g_printerr ("[on_bus_message] WARNING %d: %s\n", err->code, (err->message ? err->message : "(None)"));
    g_printerr ("[on_bus_message] Debug info: %s\n", (debug ? debug : "(None)"));
    g_error_free (err);
    g_free (debug);

    break;
  case GST_MESSAGE_EOS:
    quit = TRUE;
    break;
  default:
    return TRUE;
    break;
  }

  if (quit) {
    g_print ("[on_bus_message] End Of Stream\n");
    g_main_loop_quit (loop);
  }

  return TRUE;
}

// ----------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  // Initialize GStreamer
  gst_init (&argc, &argv);

  // Initialize the application's main loop, that handles all events
  GMainLoop *loop = g_main_loop_new (NULL, FALSE);



  /* GStreamer pipeline setup
   * ========================
   *
   * This includes setting up a bus watcher, to show messages that get posted.
   */

  GstElement *pipeline = gst_pipeline_new ("gstreamer-example-1-pipeline");

  GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  guint bus_watch_id = gst_bus_add_watch (bus, (GstBusFunc)on_bus_message, loop);
  gst_object_unref (bus);



  /* GStreamer element setup
   * =======================
   */

  GstElement *source     = gst_element_factory_make ("souphttpsrc",   "source");
  GstElement *demux      = gst_element_factory_make ("matroskademux", "demux");
  GstElement *audio_que  = gst_element_factory_make ("queue",         "audio_queue");
  GstElement *audio_dec  = gst_element_factory_make ("vorbisdec",     "audio_dec");
  GstElement *audio_sink = gst_element_factory_make ("autoaudiosink", "audio_sink");
  GstElement *video_que  = gst_element_factory_make ("queue",         "video_queue");
  GstElement *video_dec  = gst_element_factory_make ("vp8dec",        "video_dec");
  GstElement *video_sink = gst_element_factory_make ("autovideosink", "video_sink");

  if (!pipeline || !source || !demux
      || !audio_que || !audio_dec || !audio_sink
      || !video_que || !video_dec || !video_sink) {
    g_print ("[main] ERROR: Not all elements could be created\n");
    return -1;
  }

  // Configure element properties
  g_object_set (G_OBJECT (source), "location", video_uri, NULL);

  // Add all elements to the pipeline
  gst_bin_add_many (GST_BIN (pipeline), source, demux,
                    audio_que, audio_dec, audio_sink,
                    video_que, video_dec, video_sink,
                    NULL);

  /* Link elements together. Reference:
   *
   * gst-launch-1.0 \
   *   souphttpsrc location="$VIDEO_URI" \
   *   ! matroskademux name=demux \
   *   demux.audio_0 ! queue ! vorbisdec ! autoaudiosink \
   *   demux.video_0 ! queue ! vp8dec ! autovideosink
   */
  gst_element_link (source, demux);
  gst_element_link_many (audio_que, audio_dec, audio_sink, NULL);
  gst_element_link_many (video_que, video_dec, video_sink, NULL);

  /* The demuxer's pads are "sometimes", which means that they won't exist
   * before the media starts flowing.
   * We need to wait until the program is running and the demuxer emits the
   * "pad-added" signal, which lets us connect the newly added demuxer pad
   * to its corresponding decoder element.
   */
  demux_sinks_t demux_sinks = { audio_que, video_que };
  g_signal_connect (demux, "pad-added", G_CALLBACK (on_demux_pad_added),
                    &demux_sinks);

  // Set PLAYING state to pipeline and all its elements
  gst_element_set_state (pipeline, GST_STATE_PLAYING);



  /* Application main loop
   * =====================
   */

  g_print ("[main] Run the main loop...\n");
  g_main_loop_run (loop);



  /* Cleanup
   * =======
   */

  g_print ("[main] Main loop finished!\n");

  const gchar *dot_name = "pipeline";
  g_print ("[main] Export GStreamer DOT file: %s\n", dot_name);

  // Env var 'GST_DEBUG_DUMP_DOT_DIR' must be set
  GST_DEBUG_BIN_TO_DOT_FILE_WITH_TS (GST_BIN (pipeline), GST_DEBUG_GRAPH_SHOW_ALL, dot_name);

  g_print ("[main] Stop playback\n");
  gst_element_set_state (pipeline, GST_STATE_NULL);

  g_print ("[main] Delete the pipeline\n");
  gst_object_unref (GST_OBJECT (pipeline));
  g_source_remove (bus_watch_id);
  g_main_loop_unref (loop);

  return 0;
}

// ----------------------------------------------------------------------------
