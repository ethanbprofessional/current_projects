#include <gtk/gtk.h>
#include <stdio.h>
/**
 * Based on code from this tutorial: https://github.com/ToshioCP/Gtk4-tutorial/blob/main/gfm/sec5.md
 * Modified by: Ethan Bown
 * Last Modified on: 10/12/23
 * Creates a window for the puzzle program to handle UI
 * 
*/
static void app_activate (GApplication *app) {
  GtkWidget *window;
  GtkWidget *textw;
  GtkTextBuffer *textbuffer;
  gchar text[20000];
  FILE *fp;

  char buf[500];
  fp = fopen("savePuzzles.txt", "r");
  window = gtk_application_window_new (GTK_APPLICATION (app));
  // fgets(buf, 500, fp);
  gtk_window_set_title (GTK_WINDOW (window), "Puzzle");
  gtk_window_set_default_size (GTK_WINDOW (window), 400, 400);
  textw = gtk_text_view_new();
  textbuffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (textw));
  while(fgets(buf, 100, fp)) strcat(text, buf);
  gtk_text_buffer_set_text (textbuffer, text, -1);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textw), GTK_WRAP_WORD_CHAR);

  gtk_window_set_child (GTK_WINDOW (window), textw);
  gtk_window_present (GTK_WINDOW (window));
  fclose(fp);
}

int main (int argc, char **argv) {
  GtkApplication *app;

  app = gtk_application_new ("org.puzzlegtk", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (app_activate), NULL);
  int status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}