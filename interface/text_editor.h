typedef struct FileMenu {
   GtkWidget *menu_label;
   GtkWidget *menu;
   GtkWidget *new;
   GtkWidget *open;
   GtkWidget *save;
   GtkWidget *save_as;
   GtkWidget *close;
   GtkWidget *separator;
   GtkWidget *quit;
} FileMenu;

typedef struct EditMenu {
   GtkWidget *menu_label;
   GtkWidget *menu;
   GtkWidget *cut;
   GtkWidget *copy;
   GtkWidget *paste;
} EditMenu;

typedef struct OptionsMenu {
   GtkWidget *menu_label;
   GtkWidget *menu;
   GtkWidget *font;
} OptionsMenu;

typedef struct HelpMenu {
   GtkWidget *menu_label;
   GtkWidget *menu;
   GtkWidget *about;
} HelpMenu;

typedef struct FileObject {
   GtkWidget *scrolled_window;
   GtkWidget *textview;
   GtkWidget *tab_label;
   gint tab_index;
   gboolean modified;
   gchar *filename;
} FileObject;

typedef struct TextEditUI {
   GtkWidget *toplevel;
   GtkWidget *vbox;
   GtkWidget *notebook;
   GtkWidget *menubar;
   GtkWidget *toolbar;
   FileMenu *filemenu;
   EditMenu *editmenu;
   OptionsMenu *options_menu;
   HelpMenu *helpmenu;
} TextEditUI;

#define FILE_SAVED 1
#define FILE_NOT_SAVED 0
#define FILE_OBJECT(obj) ((FileObject *) obj)

static GSList *files = NULL;   /* Linked list of FileObject's */
static PangoFontDescription *desc = NULL;   /* Global font for all tabs */

static void text_edit_init_GUI(TextEditUI *);
static void text_edit_create_menus(TextEditUI *);
static void text_edit_create_toolbar_items(TextEditUI *);
static void text_edit_create_new_file(GtkNotebook *);
static void text_edit_tab_new_with_file(GtkMenuItem *, GtkNotebook *);
static void text_edit_record_buffer_state(GtkTextBuffer *, FileObject *);
static void text_edit_cut_to_clipboard(GtkMenuItem *, GtkNotebook *);
static void text_edit_copy_to_clipboard(GtkMenuItem *, GtkNotebook *);
static void text_edit_paste_from_clipboard(GtkMenuItem *, GtkNotebook *);
GSList *text_edit_get_file_list_node(gint);
FileObject *text_edit_get_file_object_from_tab_n(gint);
FileObject *text_edit_get_file_object(GtkNotebook *);
static void text_edit_open_file(GtkMenuItem *, GtkNotebook *);
static gint text_edit_get_file_selection(gchar **filename);
static void text_edit_close_file(GtkMenuItem *, GtkNotebook *);
static void text_edit_tab_new_with_file_contents(GtkNotebook *, gchar *);
static void text_edit_close_file(GtkMenuItem *, GtkNotebook *);
static gint text_edit_save_file(GtkMenuItem *, GtkNotebook *);
static gint text_edit_save_file_as(GtkMenuItem *, GtkNotebook *);
static void text_edit_remove_file(GtkNotebook *, gint);
static gint text_edit_options_prompt_on_close(void);
static void text_edit_get_font_selection(GtkMenuItem *, gpointer);
static void text_edit_apply_font_selection(void);
static void text_edit_show_about_info(GtkMenuItem *, GtkWidget *);