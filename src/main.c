#include <locale.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <gtk/gtk.h>

GtkWidget *g_tv_tweet_text;
GtkWidget *g_btn_do_tweet;

char *get_text_from_text_view(GtkWidget *text_view)
{
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *) text_view);

    gtk_text_buffer_get_bounds(buffer, &start, &end);

    return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "en_GB.UTF-8");
    GtkBuilder *builder;
    GtkWidget *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "glade/twonger.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);

    g_tv_tweet_text = GTK_WIDGET(gtk_builder_get_object(builder, "tweet_text"));
    g_btn_do_tweet = GTK_WIDGET(gtk_builder_get_object(builder, "do_tweet"));

    g_object_unref(builder);

    gtk_widget_show(window);
    gtk_main();
    
    return 0;
}

void on_do_tweet_clicked()
{    
    char *text = get_text_from_text_view(g_tv_tweet_text);
    int text_length = strlen(text);
    int tweet_no = text_length / 130 + (text_length % 130 != 0);
    char tweets[(int) tweet_no][140 * sizeof(char)];
    
    for (int i = 0; i < tweet_no; i++)
    {
        char *tweet = (char*) malloc(sizeof(char) * 140); 
        if (tweet_no != 1) {
            if (i + 1 == tweet_no)
                strcpy(tweet, &text[i * 130]);
            else
                strncpy(tweet, &text[i * 130], 130);
            tweet[130] = '\0';
            strcat(tweet, " (%d/%i)\n");
            sprintf(tweets[i], tweet, i + 1, tweet_no);
        } else {
            strcpy(tweets[i], &text[i * 130]);
        }
        
        free(tweet); 
    }

    free(text);
 
    GtkTextBuffer *buffer = gtk_text_view_get_buffer((GtkTextView *) g_tv_tweet_text);
    gtk_text_buffer_set_text(buffer, "", 0);

    for (int i = 0; i < sizeof(tweets) / sizeof(tweets[0]); i++)
    {
        gtk_text_buffer_insert_at_cursor(buffer, tweets[i], strlen(tweets[i]));
    }

    memset(tweets, 0, sizeof(tweets));
}

void on_window_main_destroy()
{
    gtk_main_quit();
}
