#!/usr/bin/env python3
# -*- coding: utf-8 -*-


import os
import sys
from tkinter import PhotoImage, messagebox, mainloop, Toplevel, Tk, Text, Menu, IntVar, StringVar, TclError, SEL_FIRST, SEL_LAST, SEL, INSERT, END, NONE, WORD, TOP, RAISED, SUNKEN, HORIZONTAL, E, W, N, S, X, BOTTOM, BOTH, LEFT
from tkinter.ttk import Entry, Frame, LabelFrame, Radiobutton, Scrollbar, Button, Label, Checkbutton, Combobox
from tkinter.filedialog import askopenfile
import subprocess

TITLE = "Compilador"
top = Tk()
if "nt" == os.name:
    top.wm_iconbitmap(bitmap = "Notepad.ico") #top.wm_iconbitmap("Notepad.ico")
else:
    try:
        img = PhotoImage(file='notepad.gif')
        root.tk.call('wm', 'iconphoto', root._w, img)
    except:
        pass   
filename = None
key_word = ""

frame = Frame(top, relief=SUNKEN)
frame.grid_rowconfigure(0, weight=1)
frame.grid_columnconfigure(0, weight=1)
xscrollbar = Scrollbar(frame, orient=HORIZONTAL)
xscrollbar.grid(row=1, column=0, sticky=E+W)
yscrollbar = Scrollbar(frame)
yscrollbar.grid(row=0, column=1, sticky=N+S)
Output = Text(frame, bg = "grey", wrap=NONE, height = 4,
            xscrollcommand=xscrollbar.set,
            yscrollcommand=yscrollbar.set)
Output.focus_set()

Output.grid(row=1, column=0, sticky=N+S+E+W, padx= 10, pady=10)
Output.config( wrap = WORD, # use word wrapping
               undo = True, # Tk 8.4 
               width = 100 )
xscrollbar.config(command=Output.xview)
yscrollbar.config(command=Output.yview)
# text widget
editor = Text(frame, wrap=NONE,
            xscrollcommand=xscrollbar.set,
            yscrollcommand=yscrollbar.set)
editor.focus_set()
editor.grid(row=0, column=0, sticky=N+S+E+W)
editor.config( wrap = WORD, # use word wrapping
               undo = True, # Tk 8.4 
               width = 100)
xscrollbar.config(command=editor.xview)
yscrollbar.config(command=editor.yview)


    
class StatusBar(Frame):
    def __init__(self, master):
        Frame.__init__(self, master)
        self.label = Label(self, relief=SUNKEN, anchor=W)
        self.label.pack(fill=X)
    def set(self, format, *args):
        self.label.config(text=format % args)
        self.label.update_idletasks()
    def clear(self):
        self.label.config(text="")
        self.label.update_idletasks()
statusbar = StatusBar(frame)
statusbar.grid(row=2, column=1, columnspan=2, sticky=E+W)
frame.pack(side=BOTTOM, fill=BOTH, expand=1)

def set_title(local_filename = None):
    if local_filename == None:
        top.wm_title("Documento sem nome" + " - " + TITLE)
    else:
        top.wm_title(os.path.basename(local_filename) + " - " + TITLE)
set_title(filename)



#micro functions
def save_if_modified():
    print("save_if_modified() editor.edit_modified()", editor.edit_modified())
    if editor.edit_modified():
        from tkinter.messagebox import askyesnocancel     
        response = askyesnocancel(TITLE, "Este Documento foi modificado. Deseja salvar as alterações?") #cancel = none, no = false, yes = true
        print("Response: ", response)
        if response:
            cancelled = save()
            print("Cancelled: ", response)
            if cancelled == 1:
                return None
            elif cancelled == 0:
                return response
        else:
            return response
    else:
        return 0
def save(): #local_filename = filename):
    local_filename = filename
    if local_filename:
        cancelled = save_as(local_filename)
    else:
        cancelled = save_as()
    return cancelled
def save_as(local_filename=None):
    if local_filename == None:        
        from tkinter.filedialog import asksaveasfilename
        local_filename = asksaveasfilename(filetypes=(('Text files', '*.txt'), ('Python files', '*.py *.pyw'), ('All files', '*.*'))) #defaultextension='.txt', 
    try:
        with open(local_filename, 'wb') as f:
                text = editor.get(1.0, END)
                f.write(bytes(text, 'UTF-8'))
                #f.write("\n") # normalize trailing whitespace
                editor.edit_modified(False)
                set_title(local_filename)
                return 0
    except FileNotFoundError:
        print('FileNotFoundError')
        return 1
def close():
    response = save_if_modified()
    if response != None: #None = Cancel save before new/open/quit confirmation, True = Save and quit, False = Quit without saving
        top.destroy() #sys.exit(0)
#file menu functions
def file_new(event = None):
    response = save_if_modified()
    if response != None:
        editor.delete(1.0, END)
        editor.edit_modified(False)
        global filename
        filename = None
        set_title()
        editor.edit_reset() 
def file_open(event = None, local_filename = None):
    response = save_if_modified()
    if response != None:
        # Returns the opened file
        if local_filename == None:
            from tkinter.filedialog import askopenfilename
            local_filename = askopenfilename()
        if local_filename != None  and local_filename != '':
            with open(local_filename, 'rb') as f:
                fileContents = f.read()# Get all the text from file.           
            # Set current text to file contents
            editor.delete(1.0, END)
            editor.insert(1.0, fileContents)
            editor.edit_modified(False)
            set_title(local_filename)
        global filename
        filename = local_filename
        return 'break'
if len(sys.argv) > 1:
    filename = sys.argv[1]
    file_open(local_filename = filename)
def file_save(event = None):
    save()
def file_save_as(event = None):
    save_as()
def file_quit(event = None):
    close() #sys.exit(0)
#edit menu functions
def edit_copy(event=None):
    print('rs.get()=', rs.get())
    if rs.get() == 1:
        editor.clipboard_clear()
        clipped_text = rect_sel_end()
        print('clipped_text=', clipped_text)
        editor.clipboard_append(clipped_text)
    else:
        try:
            editor.clipboard_clear()
            clipped_text = editor.get("sel.first", "sel.last")
            editor.clipboard_append(clipped_text)
        except TclError:
            pass
    return 'break'
def edit_cut(event=None):
    edit_copy()
    edit_delete()
def edit_paste(event=None):
    try:
        clipped_text = editor.selection_get(selection='CLIPBOARD')
        editor.insert('insert', clipped_text)
    except:
        pass
    return 'break'
def edit_delete(event=None):
    if rs.get() == 1:
        clipped_text = rect_sel_end()
        if clipped_text != None:
            rect_sel_end(delete=1)
            return 'break'
def edit_select_all(event = None):
    editor.tag_add(SEL, "1.0", END)
    editor.mark_set(INSERT, "1.0")
    editor.see(INSERT)
    return 'break'
def edit_find(event=None):
    try:
        editor.tag_delete("search")
    except:
        pass
    self = Toplevel(top)
    self.searchlabel = Label(self, text="Find what:")
    self.searchlabel.pack()
    self.key_word_entry = Entry(self, text="Find something...")
    self.key_word_entry.pack()
    self.matchcasevar = IntVar()
    self.matchcasevar.set(1)
    self.matchcase = Checkbutton(self, text="Match case", variable=self.matchcasevar)
    self.matchcase.pack()
    self.searcharea = StringVar()
    self.searcharea.set("all")
    self.group = LabelFrame(self, text="Direction", padx=5, pady=5)
    self.group.pack(padx=1, pady=3, anchor=W)
    Radiobutton(self.group, text="Up", variable=self.searcharea, value="up").pack(anchor=W)
    Radiobutton(self.group, text="Down", variable=self.searcharea, value="down").pack(anchor=W)
    Radiobutton(self.group, text="All", variable=self.searcharea, value="all").pack(anchor=W)
    self.key_word_entry.focus()
    def get_parameters(reply):
        key_word = self.key_word_entry.get()
        if self.searcharea.get() == "up":
            self.search_from = "1.0"
            self.search_until = INSERT
        elif self.searcharea.get() == "down":
            self.search_from = INSERT
            self.search_until = END
        elif self.searcharea.get() == "all":
            self.search_from = "1.0"
            self.search_until = END
        self.case_sensitive = self.matchcasevar.get()
        if reply == 1:
            answer = [key_word, self.search_from, self.search_until, self.case_sensitive]
            return answer
    get_parameters(reply=0)
    def findnextcmd(top=top, self=self, event=None):
        try:
            editor.tag_delete("current_word")
        except:
            pass
        answer = get_parameters(reply=1)
        key_word = answer[0]
        search_from = answer[1]
        search_until = answer[2]
        case_sensitive = answer[3]
        while True:
            start_kw = editor.search(key_word, search_from, stopindex=search_until, nocase=case_sensitive)
            if start_kw == "" or start_kw == None:
                try:
                    start_kw = editor.search(key_word, answer[1], stopindex=search_until, nocase=case_sensitive)    
                    editor.tag_configure("current_word", background="sea green")
                    editor.tag_add("current_word", start_kw, "%s + %sc"%(start_kw, len(key_word)))        
                    editor.see("%s + %sc"%(start_kw, len(key_word)))
                except:
                    pass
                break
            editor.tag_configure("search", background="green")
            editor.tag_add("search", start_kw, "%s + %sc"%(start_kw, len(key_word)))
            search_from = "%s + %sc "%(start_kw, len(key_word))
    self.findnext = Button(self, text="Find Next", command=findnextcmd)
    self.findnext.pack(anchor=W)
    self.bind("<Return>", findnextcmd)
    def cancelcmd(self=self):
        self.unbind("<Return>")
        editor.focus()
        self.destroy()
    self.cancel = Button(self, text="Cancel", command=cancelcmd)
    self.cancel.pack(anchor=W)
def edit_goto(event=None):
    from tkinter.simpledialog import askinteger
    line = askinteger('Goto', 'Enter line number:')
    editor.update()
    editor.focus()
    if line is not None:
        maxindex = editor.index(END+'-1c')
        maxline  = int(maxindex.split('.')[0])
        if line > 0 and line <= maxline:
            editor.mark_set(INSERT, '%d.0' % line)      # goto line
            editor.tag_remove(SEL, '1.0', END)          # delete selects
            editor.tag_add(SEL, INSERT, 'insert + 1l')  # select line
            editor.text.see(INSERT)                          # scroll to line
        else:
            from tkinter.messagebox import showerror
            showerror('PyEdit', 'Bad line number')
#other functions
def hello():
    messagebox.showinfo("Ok!")
    
def compilar():
    a=save()
    print(filename)
    
    x = subprocess.call(["./comp",filename[:-4], "file"])
    out = open("output.txt", "r")
    for linha in out.readlines():
        Output.insert(INSERT, linha)
#instead of closing the window, execute a function 
top.protocol("WM_DELETE_WINDOW", close) 

#create a top level menu
menubar = Menu(top)
#Menu item File
filemenu = Menu(menubar, tearoff=0)# tearoff = 0 => can't be seperated from window
filemenu.add_command(label="Novo", underline=1, command=file_new, accelerator="Ctrl+N")
filemenu.add_command(label="Abrir...", underline=1, command=file_open, accelerator="Ctrl+O")
filemenu.add_command(label="Salvar", underline=1, command=file_save, accelerator="Ctrl+S")
filemenu.add_command(label="Salvar como..", underline=5, command=file_save_as, accelerator="Ctrl+Alt+S")
filemenu.add_separator()
filemenu.add_command(label="Sair", underline=2, command=file_quit, accelerator="Alt+F4")
menubar.add_cascade(label="Arquivo", underline=0, menu=filemenu)
#Menu item Edit
editmenu = Menu(menubar, tearoff=0)
editmenu.add_command(label="Desfazer", underline=0, command=editor.edit_undo, accelerator="Ctrl+Z")
editmenu.add_command(label="Refazer", underline=0, command=editor.edit_redo, accelerator="Ctrl+Y")
editmenu.add_separator()
editmenu.add_command(label="Recorta", underline=2, command=edit_cut, accelerator="Ctrl+X")
editmenu.add_command(label="Copia", underline=0, command=edit_copy, accelerator="Ctrl+C")
editmenu.add_command(label="Cola", underline=0, command=edit_paste, accelerator="Ctrl+V")
editmenu.add_command(label="Delete", underline=2, command=edit_delete, accelerator="Del")
editmenu.add_separator()

menubar.add_cascade(label="Editar", underline=0, menu=editmenu)

construirmenu = Menu(menubar, tearoff=0)
construirmenu.add_command(label="Compilar", underline=0, command=compilar, accelerator="F5")
construirmenu.add_command(label="Compilar e Executar", underline=0, command=editor.edit_redo, accelerator="Alt+F5")
menubar.add_cascade(label = "Construir", underline =0 , menu=construirmenu)
# display the menu
top.config(menu=menubar)
# create keyboard shortcuts for functions
editor.bind("<Control-n>", file_new)
editor.bind("<Control-o>", file_open)
editor.bind("<Control-s>", file_save)
editor.bind("<Control-Shift-s>", file_save_as)
editor.bind("<Control-q>", file_quit)
editor.bind("<Control-N>", file_new)
editor.bind("<Control-O>", file_open)
editor.bind("<Control-S>", file_save)
editor.bind("<Control-Shift-S>", file_save_as)
editor.bind("<Control-Q>", file_quit)
editor.bind("<Control-a>", edit_select_all)
editor.bind("<Control-A>", edit_select_all)
editor.bind("<Control-f>", edit_find)
editor.bind("<Control-F>", edit_find)
editor.bind("<Control-C>", edit_copy)
editor.bind("<Control-c>", edit_copy)
editor.bind("<Control-V>", edit_paste)
editor.bind("<Control-v>", edit_paste)
editor.bind("<Control-X>", edit_cut)
editor.bind("<Control-x>", edit_cut)
editor.bind("<BackSpace>", edit_delete)
editor.bind("<Delete>", edit_delete)
editor.bind("<Control-G>", edit_goto)
editor.bind("<Control-g>", edit_goto)
#h1 -h6 shortcuts

#context menu
cmenu = Menu(top, tearoff=0)
cmenu.add_command(label="Undo", command=editor.edit_undo)
cmenu.add_separator()
cmenu.add_command(label="Cut", command=edit_cut)
cmenu.add_command(label="Copy", command=edit_copy)
cmenu.add_command(label="Paste", command=edit_paste)
cmenu.add_command(label="Delete", command=edit_delete)
cmenu.add_separator()
cmenu.add_command(label="Select all", command=edit_select_all)
cmenu.add_separator()
cmenu.add_command(label="Exit", command=file_quit)
def context_menu(event):
    cmenu.post(event.x_root, event.y_root)
editor.bind("<Button-3>", context_menu)



#Output.pack()

mainloop()