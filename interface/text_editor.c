#include <gtk-3.0/gtk/gtk.h>
#include <gtk-3.0/gdk/gdkkeysyms.h>
#include <string.h>
#include <libgen.h>
#include "text_editor.h"

int main(int argc, char *argv[])
{
   TextEditUI app;

   gtk_init(&argc, &argv);

   app.toplevel = gtk_window_new(GTK_WINDOW_TOPLEVEL);   /* Main window */

   gtk_window_set_title(GTK_WINDOW(app.toplevel), "TextEdit");
   gtk_window_set_default_size(GTK_WINDOW(app.toplevel), 650, 350);
   gtk_window_set_icon_from_file(GTK_WINDOW(app.toplevel), "icons/texteditor.png", NULL);

   /*
    * Connect signal handler for destruction of
    * top-level window.
    */
   g_signal_connect(G_OBJECT(app.toplevel), "destroy",
                    G_CALLBACK(gtk_main_quit), NULL);

   text_edit_init_GUI(&app);   /* Build interface */

   gtk_widget_show_all(app.toplevel);

   gtk_main();

   return 0;
}

static void text_edit_init_GUI(TextEditUI *app)
{
   desc = pango_font_description_from_string("Progsole normal 12");

   app->vbox = gtk_vbox_new(FALSE, 0);
   app->notebook = gtk_notebook_new();
   app->menubar = gtk_menu_bar_new();

   text_edit_create_menus(app);
   text_edit_create_toolbar_items(app);
   text_edit_create_new_file((GtkNotebook *) app->notebook);

   gtk_notebook_set_show_tabs(GTK_NOTEBOOK(app->notebook), TRUE);
   gtk_box_pack_start(GTK_BOX(app->vbox), app->notebook, TRUE, TRUE, 0);
   gtk_container_add(GTK_CONTAINER(app->toplevel), app->vbox);
}

static void text_edit_create_menus(TextEditUI *app)
{
   FileMenu *file;
   EditMenu *edit;
   OptionsMenu *options;
   HelpMenu *help;
   GtkAccelGroup *group = gtk_accel_group_new();

   app->filemenu = g_new(FileMenu, 1);
   app->editmenu = g_new(EditMenu, 1);
   app->options_menu = g_new(OptionsMenu, 1);
   app->helpmenu = g_new(HelpMenu, 1);

   file = app->filemenu;
   edit = app->editmenu;
   options = app->options_menu;
   help = app->helpmenu;

   gtk_window_add_accel_group(GTK_WINDOW(app->toplevel), group);

   file->menu_label = gtk_menu_item_new_with_label("File");
   file->menu = gtk_menu_new();
   file->new = gtk_image_menu_item_new_from_stock(GTK_STOCK_NEW, group);
   file->open = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, group);
   file->save = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, group);
   file->save_as = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE_AS, group);
   file->close = gtk_image_menu_item_new_from_stock(GTK_STOCK_CLOSE, group);
   file->separator = gtk_separator_menu_item_new();
   file->quit = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, group);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(file->menu_label), file->menu);
   gtk_menu_shell_append(GTK_MENU_SHELL(file->menu), file->new);
   gtk_menu_shell_append(GTK_MENU_SHELL(file->menu), file->open);
   gtk_menu_shell_append(GTK_MENU_SHELL(file->menu), file->save);
   gtk_menu_shell_append(GTK_MENU_SHELL(file->menu), file->save_as);
   gtk_menu_shell_append(GTK_MENU_SHELL(file->menu), file->close);
   gtk_menu_shell_append(GTK_MENU_SHELL(file->menu), file->separator);
   gtk_menu_shell_append(GTK_MENU_SHELL(file->menu), file->quit);
   gtk_menu_set_accel_group(GTK_MENU(file->menu), group);
   gtk_widget_add_accelerator(file->new, "activate", group, GDK_N,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(file->open, "activate", group, GDK_O,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(file->save, "activate", group, GDK_S,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(file->save_as, "activate", group, GDK_A,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(file->close, "activate", group, GDK_W,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(file->quit, "activate", group, GDK_Q,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


   edit->menu_label = gtk_menu_item_new_with_label("Edit");
   edit->menu = gtk_menu_new();
   edit->cut = gtk_image_menu_item_new_from_stock(GTK_STOCK_CUT, group);
   edit->copy = gtk_image_menu_item_new_from_stock(GTK_STOCK_COPY, group);
   edit->paste = gtk_image_menu_item_new_from_stock(GTK_STOCK_PASTE, group);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(edit->menu_label), edit->menu);
   gtk_menu_shell_append(GTK_MENU_SHELL(edit->menu), edit->cut);
   gtk_menu_shell_append(GTK_MENU_SHELL(edit->menu), edit->copy);
   gtk_menu_shell_append(GTK_MENU_SHELL(edit->menu), edit->paste);
   gtk_menu_set_accel_group(GTK_MENU(edit->menu), group);
   gtk_widget_add_accelerator(edit->cut, "activate", group, GDK_X,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(edit->copy, "activate", group, GDK_C,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
   gtk_widget_add_accelerator(edit->paste, "activate", group, GDK_V,
                              GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


   options->menu_label = gtk_menu_item_new_with_label("Options");
   options->menu = gtk_menu_new();
   options->font = gtk_image_menu_item_new_from_stock(GTK_STOCK_SELECT_FONT, NULL);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(options->menu_label), options->menu);
   gtk_menu_shell_append(GTK_MENU_SHELL(options->menu), options->font);


   help->menu_label = gtk_menu_item_new_with_label("Help");
   help->menu = gtk_menu_new();
   help->about = gtk_image_menu_item_new_from_stock(GTK_STOCK_ABOUT, NULL);
   gtk_menu_item_set_submenu(GTK_MENU_ITEM(help->menu_label), help->menu);
   gtk_menu_shell_append(GTK_MENU_SHELL(help->menu), help->about);


   gtk_menu_shell_append(GTK_MENU_SHELL(app->menubar), file->menu_label);
   gtk_menu_shell_append(GTK_MENU_SHELL(app->menubar), edit->menu_label);
   gtk_menu_shell_append(GTK_MENU_SHELL(app->menubar), options->menu_label);
   gtk_menu_shell_append(GTK_MENU_SHELL(app->menubar), help->menu_label);

   g_signal_connect(G_OBJECT(file->new), "activate",
                    G_CALLBACK(text_edit_tab_new_with_file), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(file->open), "activate",
                    G_CALLBACK(text_edit_open_file), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(file->save), "activate",
                    G_CALLBACK(text_edit_save_file), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(file->save_as), "activate",
                    G_CALLBACK(text_edit_save_file_as), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(file->close), "activate",
                    G_CALLBACK(text_edit_close_file), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(file->quit), "activate",
                    G_CALLBACK(gtk_main_quit), NULL);

   g_signal_connect(G_OBJECT(edit->cut), "activate",
                    G_CALLBACK(text_edit_cut_to_clipboard), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(edit->copy), "activate",
                    G_CALLBACK(text_edit_copy_to_clipboard), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(edit->paste), "activate",
                    G_CALLBACK(text_edit_paste_from_clipboard), (gpointer) app->notebook);

   g_signal_connect(G_OBJECT(options->font), "activate",
                    G_CALLBACK(text_edit_get_font_selection), NULL);

   g_signal_connect(G_OBJECT(help->about), "activate",
                    G_CALLBACK(text_edit_show_about_info), app->toplevel);

   /* Add the menubar to the vertical container for the main window */
   gtk_box_pack_start(GTK_BOX(app->vbox), app->menubar, FALSE, FALSE, 0);
}

static void text_edit_create_toolbar_items(TextEditUI *app)
{
   GtkWidget *toolbar;
   GtkToolItem *new, *open, *save;

   toolbar = app->toolbar = gtk_toolbar_new();
   gtk_toolbar_set_show_arrow(GTK_TOOLBAR(toolbar), TRUE);
   new = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
   open = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
   save = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
   gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new, 0);
   gtk_toolbar_insert(GTK_TOOLBAR(toolbar), open, 1);
   gtk_toolbar_insert(GTK_TOOLBAR(toolbar), save, 2);

   g_signal_connect_swapped(G_OBJECT(new), "clicked",
                            G_CALLBACK(gtk_menu_item_activate),
                            (gpointer) app->filemenu->new);

   g_signal_connect_swapped(G_OBJECT(open), "clicked",
                            G_CALLBACK(gtk_menu_item_activate),
                            (gpointer) app->filemenu->open);

   g_signal_connect_swapped(G_OBJECT(save), "clicked",
                            G_CALLBACK(gtk_menu_item_activate),
                            (gpointer) app->filemenu->save);

   gtk_box_pack_start(GTK_BOX(app->vbox), toolbar, FALSE, FALSE, 0);
}

static void text_edit_create_new_file(GtkNotebook *notebook)
{
   FileObject *new_file = g_new(FileObject, 1);
   GtkTextBuffer *buf;

   new_file->scrolled_window = gtk_scrolled_window_new(NULL, NULL);
   new_file->textview = gtk_text_view_new();
   new_file->tab_label = gtk_label_new("Untitled");
   new_file->modified = FALSE;
   new_file->filename = "UntitledDocument";

   /**
    * Retreive the buffer from the textview and connect a signal
    * handler so that we can keep track of changes to the buffer
    * and know when to prompt a user to save changes.
    */

   buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(new_file->textview));

   g_signal_connect(G_OBJECT(buf), "changed",
                    G_CALLBACK(text_edit_record_buffer_state), (gpointer) new_file);

   gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(new_file->scrolled_window),
                                  GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
   gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(new_file->scrolled_window),
                                       GTK_SHADOW_IN);
   gtk_container_set_border_width(GTK_CONTAINER(new_file->scrolled_window), 3);
   gtk_text_view_set_left_margin(GTK_TEXT_VIEW(new_file->textview), 3);
   gtk_text_view_set_right_margin(GTK_TEXT_VIEW(new_file->textview), 3);
   gtk_text_view_set_pixels_above_lines(GTK_TEXT_VIEW(new_file->textview), 1);
   gtk_widget_modify_font(new_file->textview, desc);   /* desc is global font description */

   /**
    * Add each file to the linked list of files
    * so that we can keep track of what we have open
    * and to access them through the notebook of the
    * application.
    */
   files = g_slist_append(files, new_file);

   gtk_container_add(GTK_CONTAINER(new_file->scrolled_window), new_file->textview);

   new_file->tab_index = gtk_notebook_append_page(notebook, new_file->scrolled_window,
                                                  new_file->tab_label);

   gtk_widget_show_all(GTK_WIDGET(notebook));
}

static void text_edit_tab_new_with_file(GtkMenuItem *menu_item, GtkNotebook *notebook)
{ text_edit_create_new_file(notebook); }

static void text_edit_record_buffer_state(GtkTextBuffer *buffer, FileObject *file)
{
   /**
    * Here we will record state changes to the buffer
    * between saves.  This will allow us to offer the user
    * a dialog before exiting if they have not saved the file
    * after the last time it was modified.
    * NOTE: After a save operation, the 'modified' property will
    * be set back to false.
    */
   if (file->modified == FALSE) file->modified = TRUE;
   else
      return;
}

static void text_edit_cut_to_clipboard(GtkMenuItem *menu_item, GtkNotebook *notebook)
{
   GList *children;
   GtkWidget *textview;
   GtkTextBuffer *buffer;
   GtkWidget *scrolled_window;
   GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
   gint page;

   page = gtk_notebook_get_current_page(notebook);
   scrolled_window = gtk_notebook_get_nth_page(notebook, page);
   children = gtk_container_get_children(GTK_CONTAINER(scrolled_window));
   textview = children->data;

   buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
   gtk_text_buffer_cut_clipboard(buffer, clipboard, TRUE);
}

static void text_edit_copy_to_clipboard(GtkMenuItem *menu_item, GtkNotebook *notebook)
{
   GList *children;
   GtkWidget *textview;
   GtkTextBuffer *buffer;
   GtkWidget *scrolled_window;
   GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
   gint page;

   page = gtk_notebook_get_current_page(notebook);
   scrolled_window = gtk_notebook_get_nth_page(notebook, page);
   children = gtk_container_get_children(GTK_CONTAINER(scrolled_window));
   textview = children->data;

   buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
   gtk_text_buffer_copy_clipboard(buffer, clipboard);
}

static void text_edit_paste_from_clipboard(GtkMenuItem *menu_item, GtkNotebook *notebook)
{
   GList *children;
   GtkWidget *textview;
   GtkTextBuffer *buffer;
   GtkWidget *scrolled_window;
   GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
   gint page;

   page = gtk_notebook_get_current_page(notebook);
   scrolled_window = gtk_notebook_get_nth_page(notebook, page);
   children = gtk_container_get_children(GTK_CONTAINER(scrolled_window));
   textview = children->data;

   buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
   gtk_text_buffer_paste_clipboard(buffer, clipboard, NULL, TRUE);
}

GSList *text_edit_get_file_list_node(gint index)
{
   GSList *node;

   for (node = files; node != NULL; node = node->next)
      if (FILE_OBJECT(node->data)->tab_index == index)
         return node;

   return NULL;
}

FileObject *text_edit_get_file_object_from_tab_n(gint index)
{
   GSList *node = text_edit_get_file_list_node(index);

   g_return_val_if_fail(node != NULL, NULL);

   return FILE_OBJECT(node->data);
}

FileObject *text_edit_get_file_object(GtkNotebook *notebook)
{
   FileObject *obj;
   gint index;

   index = gtk_notebook_get_current_page(notebook);

   obj = text_edit_get_file_object_from_tab_n(index);

   g_return_val_if_fail(obj != NULL, NULL);

   return obj;
}

static void text_edit_open_file(GtkMenuItem *menu_item, GtkNotebook *notebook)
{
   FileObject *file_obj;
   gchar *filename;
   gchar *contents;
   GtkTextIter start;
   GtkTextMark *mark;
   GtkTextBuffer *buffer;

   switch (text_edit_get_file_selection(&filename))
    {
      case GTK_RESPONSE_ACCEPT:
       {
         if (gtk_notebook_get_n_pages(notebook) == 0)
          {
            text_edit_tab_new_with_file_contents(notebook, filename);
            return;
          }
         else
            file_obj = text_edit_get_file_object(notebook);

         if ((strcmp(file_obj->filename, "UntitledDocument") != 0) || (file_obj->modified))
          {
            /**
             * If the file currently in this tab has a name or has
             * been modified since the last save, then we don't want
             * to replace its contents with something else.  Let's just
             * open a new tab for the file the user wants to open.
             */
            text_edit_tab_new_with_file_contents(notebook, filename);
            return;
          }
         else
          {
            file_obj->filename = filename;

            g_file_get_contents(filename, &contents, NULL, NULL);

            buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(file_obj->textview));
            mark = gtk_text_buffer_get_insert(buffer);
            gtk_text_buffer_get_iter_at_mark(buffer, &start, mark);

            gtk_text_buffer_set_text(buffer, contents, -1);

            /**
             * The 'modified' property should already be set,
             * but we set it here again since even though the GtkTextBuffer
             * has changed (i.e. file was opened and text inserted) we still
             * haven't really edited the file yet, which is what the property
             * is supposed to indicate.
             */
            file_obj->modified = FALSE;

            gtk_label_set_text(GTK_LABEL(file_obj->tab_label), basename(filename));
          }
         break;
       }
      case GTK_RESPONSE_REJECT:
      case GTK_FILE_CHOOSER_ERROR_NONEXISTENT:
         break;
    }
}

static gint text_edit_get_file_selection(gchar **filename)
{
   GtkWidget *dialog;
   GtkWidget *error;
   gint id;

   dialog = gtk_file_chooser_dialog_new("Open A File", NULL,
                                        GTK_FILE_CHOOSER_ACTION_OPEN,
                                        GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
                                        GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                        NULL);

   if ((id = gtk_dialog_run(GTK_DIALOG(dialog))) == GTK_RESPONSE_ACCEPT)
    {
      *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

      if (!g_file_test(*filename, G_FILE_TEST_EXISTS))
       {
         error = gtk_message_dialog_new_with_markup(NULL, GTK_DIALOG_MODAL,
                                GTK_MESSAGE_ERROR,
                                GTK_BUTTONS_OK,
                                "<span font_weight='bold'>File does not exist!</span>\n");

         gtk_widget_destroy(dialog);
         gtk_dialog_run(GTK_DIALOG(error));
         gtk_widget_destroy(error);

         *filename = NULL;

         return GTK_FILE_CHOOSER_ERROR_NONEXISTENT;
       }
    }

   gtk_widget_destroy(dialog);

   return id;
}    

static void text_edit_close_file(GtkMenuItem *menu_item, GtkNotebook *notebook)
{
   FileObject *file_obj;
   gint id;
   gint saved;

   if (gtk_notebook_get_n_pages(notebook) == 0)
      return;

   file_obj = text_edit_get_file_object(notebook);

   if (file_obj->modified)
    {
      id = text_edit_options_prompt_on_close();

      switch (id)
       {
         case GTK_RESPONSE_ACCEPT:
          {
            if (strcmp(file_obj->filename, "UntitledDocument") == 0)
               saved = text_edit_save_file_as(NULL, notebook);
            else
               saved = text_edit_save_file(NULL, notebook);

            if (saved)
               text_edit_remove_file(notebook, gtk_notebook_get_current_page(notebook));

            break;
          }
         case GTK_RESPONSE_CLOSE:
          {
            text_edit_remove_file(notebook, gtk_notebook_get_current_page(notebook));
            break;
          }
         case GTK_RESPONSE_CANCEL:
            break;
       }
    }
   else
      text_edit_remove_file(notebook, gtk_notebook_get_current_page(notebook));
}

static void text_edit_remove_file(GtkNotebook *notebook, gint index)
{
   GSList *element = text_edit_get_file_list_node(index);

   if (gtk_notebook_get_n_pages(notebook) == 1)
    {
      gtk_notebook_remove_page(notebook, index);

      files = g_slist_delete_link(files, element);
    }
   else if (element == g_slist_last(files))
    {
      gtk_notebook_remove_page(notebook, index);

      files = g_slist_delete_link(files, g_slist_last(files));
    }
   else
    {
      gtk_notebook_remove_page(notebook, index);

      GSList *iterator = g_slist_next(element);

      for ( ; iterator != NULL; iterator = g_slist_next(iterator))
         FILE_OBJECT(iterator->data)->tab_index -= 1;

      files = g_slist_delete_link(files, element);
    }
}

static void text_edit_tab_new_with_file_contents(GtkNotebook *notebook, gchar *name)
{
   FileObject *file_obj;
   gint index;
   gchar *contents;
   GtkTextIter start;
   GtkTextMark *mark;
   GtkTextBuffer *buffer;

   text_edit_create_new_file(notebook);

   index = gtk_notebook_get_n_pages(notebook) - 1;

   file_obj = text_edit_get_file_object_from_tab_n(index);

   g_file_get_contents(name, &contents, NULL, NULL);

   file_obj->filename = name;

   buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(file_obj->textview));
   mark = gtk_text_buffer_get_insert(buffer);
   gtk_text_buffer_get_iter_at_mark(buffer, &start, mark);

   gtk_text_buffer_set_text(buffer, contents, -1);

   /**
    * The 'modified' property should already be set,
    * but we set it here again since even though the GtkTextBuffer
    * has changed (i.e. file was opened and text inserted) we still
    * haven't really edited the file yet, which is what the property
    * is supposed to indicate.
    */
   file_obj->modified = FALSE;

   gtk_label_set_text(GTK_LABEL(file_obj->tab_label), basename(name));

   gtk_notebook_set_current_page(notebook, index);
}

static gint text_edit_save_file(GtkMenuItem *menu_item, GtkNotebook *notebook)
{
   FileObject *file_obj;
   GtkTextBuffer *buffer;
   GtkTextIter start, end;
   gchar *text;

   file_obj = text_edit_get_file_object(notebook);

   buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(file_obj->textview));
   gtk_text_buffer_get_bounds(buffer, &start, &end);
   text = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);

   g_file_set_contents(file_obj->filename, text, -1, NULL);

   file_obj->modified = FALSE;

   return FILE_SAVED;
}

static gint text_edit_save_file_as(GtkMenuItem *menu_item, GtkNotebook *notebook)
{
   GtkWidget *dial;
   gint response;
   gchar *selection;
   GtkTextIter start, end;
   gchar *text;
   gint action;

   dial = gtk_file_chooser_dialog_new("Save File As", NULL,
                                      GTK_FILE_CHOOSER_ACTION_SAVE,
                                      GTK_STOCK_CANCEL, GTK_RESPONSE_REJECT,
                                      GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                                      NULL);

   if ((response = gtk_dialog_run(GTK_DIALOG(dial))) == GTK_RESPONSE_ACCEPT)
    {
      selection = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dial));

      g_return_val_if_fail(selection != NULL, FILE_NOT_SAVED);

      FileObject *obj = text_edit_get_file_object(notebook);

      obj->filename = selection;

      GtkTextBuffer *buf = gtk_text_view_get_buffer(GTK_TEXT_VIEW(obj->textview));

      gtk_text_buffer_get_bounds(buf, &start, &end);
      text = gtk_text_buffer_get_text(buf, &start, &end, FALSE);

      g_file_set_contents(obj->filename, text, -1, NULL);

      gtk_label_set_text(GTK_LABEL(obj->tab_label), basename(selection));

      action = FILE_SAVED;
    }
   else if (response == GTK_RESPONSE_REJECT) action = FILE_NOT_SAVED;

   gtk_widget_destroy(dial);

   return action;
}

static gint text_edit_options_prompt_on_close(void)
{
   gint id;
   GtkWidget *option_dialog;

   option_dialog = gtk_message_dialog_new_with_markup(NULL, GTK_DIALOG_MODAL,
                                                      GTK_MESSAGE_INFO,
                                                      GTK_BUTTONS_NONE,
                                                      "<span font_weight='bold'>Save Changes?</span>\n\n"
                                                      "If you don't save, all changes will be lost.\n");

   gtk_dialog_add_buttons(GTK_DIALOG(option_dialog),
                          GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                          GTK_STOCK_DISCARD, GTK_RESPONSE_CLOSE,
                          GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
                          NULL);

   gtk_widget_show_all(option_dialog);

   id = gtk_dialog_run(GTK_DIALOG(option_dialog));

   gtk_widget_destroy(option_dialog);

   return id;
}

static void text_edit_get_font_selection(GtkMenuItem *menu_item, gpointer data)
{
   GtkWidget *font_window = gtk_font_selection_dialog_new("Choose A Font");
   gchar *fontname;
   gint id;

   gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(font_window),
                                              "abcdefghijk ABCDEFHIJK");

   switch (id = gtk_dialog_run(GTK_DIALOG(font_window)))
    {
      case GTK_RESPONSE_OK:
      case GTK_RESPONSE_APPLY:
       {
         fontname = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(font_window));
         desc = pango_font_description_from_string(fontname);
         text_edit_apply_font_selection();
         break;
       }
      case GTK_RESPONSE_CANCEL: break;
    }

   gtk_widget_destroy(font_window);
}

static void text_edit_apply_font_selection(void)
{
   GSList *node;

   for (node = files; node != NULL; node = node->next)
      gtk_widget_modify_font(FILE_OBJECT(node->data)->textview, desc);
}

static void text_edit_show_about_info(GtkMenuItem *menu_item, GtkWidget *toplevel)
{
   const gchar *authors[] = { "Glenn Schemenauer (GSchemenauer3@gmail.com)", NULL };

   const gchar *license_text = "This program is free software; you can redistribute it and/or modify "
                               "it under the terms of the GNU General Public License as published by "
                               "the Free Software Foundation, either version 3 of the License, or "
                               "(at your option) any later version.\n\n"
                               "This program is distributed in the hope that it will be useful, "
                               "but WITHOUT ANY WARRANTY; without even the implied warranty of "
                               "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the "
                               "GNU General Public License for more details.\n\n"
                               "You should have received a copy of the GNU General Public License "
                               "along with this program.  If not, see\nhttp://www.gnu.org/license/.";

   gtk_show_about_dialog(GTK_WINDOW(toplevel), "program-name", "Text Edit v1.0",
                         "authors", authors,
                         "license", license_text,
                         "comments", "A simple lightweight GTK+ text editor",
                         "wrap-license", TRUE,
                         NULL);
}