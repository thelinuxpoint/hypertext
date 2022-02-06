#include "../header/hypwindow.h"

/* The Constructor:
 *
 *
 *
 *
 */ 
hyp::HypWindow::HypWindow(): Gtk::ApplicationWindow(){
    
    set_title("Hyper Text");
    set_default_size(1000, 650);

    Gdk::RGBA black_backk;
    black_backk.set("#151515");
    override_background_color(black_backk);
    
    Gdk::Color c;
    c.set_rgb(24,25,21);

    Glib::ustring data;

    data="textview text {color : white; font-size:50px; font-family:monospace; font-weight:bold;} "; 

    nb.set_scrollable(true);

    auto css = Gtk::CssProvider::create();

    if(not css->load_from_data(data)) {
        std::cerr << "Failed to load css\n";
        std::exit(1);
    }
    auto screen = Gdk::Screen::get_default();
    auto ctx = grand_window.get_style_context();
    ctx->add_provider_for_screen(screen, css, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    for_tree = Gtk::manage(new Gtk::ScrolledWindow());

    

   
    types = new std::map<int,std::string>();
    text_track = new std::map<int,int>();
    img_track = new std::map<int,int>();

    hyp_dispatch.connect(sigc::mem_fun(*this,&hyp::HypWindow::on_thread_call));



    treeView = Gtk::manage(new hyp::HypTreeView(this));

    // m_TreeView->signal_row_expanded().connect( sigc::mem_fun(*this,&hyp::HypWindow::on_tree_click));
    // m_TreeView->signal_row_collapsed().connect( sigc::mem_fun(*this,&hyp::HypWindow::on_tree_click));  

    nb.override_background_color(black_backk);
    nb.signal_switch_page().connect( sigc::mem_fun(*this,&hyp::HypWindow::on_tab_change));
    // nb.
    //##########################################################################    
    // ADD #####################################################################


    // treeview to scrollbar
    for_tree->add(*treeView);
    for_tree->set_vexpand();

    //scrollbar to vbox
    // tree.add(*for_tree);
    //vbox to middle window
    for_shell = Gtk::manage(new Gtk::ScrolledWindow());



    v_window.pack1(nb,true,false);
    v_window.add2(*for_shell);


    v_window.set_position(v_window.property_max_position());
    v_window.set_wide_handle();

    middle_window.add1(*for_tree);


    middle_window.add2(v_window);

    // notebook to middle window
    middle_window.set_position(0);

    //##########################################################################
    // Edit menu ################################################################

    // add_action("copy", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));

    add_action("openfolder", sigc::mem_fun(*this, &hyp::HypWindow::on_folder_open));

    add_action("openfile", sigc::mem_fun(*this, &hyp::HypWindow::on_file_open));
    
    add_action("newfile", sigc::mem_fun(*this, &hyp::HypWindow::insert_tab));

    add_action("save", sigc::mem_fun(*this, &hyp::HypWindow::on_save));

    add_action("saveas", sigc::mem_fun(*this, &hyp::HypWindow::on_save_as));


    //###########################################################################

    //Choices menus, to demonstrate Radio items,
    //using our convenience methods for string and int radio values:
    m_refToggle = add_action_bool("sometoggle",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_toggle), false);
    //Help menu:
    // add_action("about", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));
    
    //##########################################################################
    //Create the toolbar and add it to a container widget:
    //##########################################################################
    file = Gtk::manage(new Gtk::Label("Plain Text"));
    status = Gtk::manage(new Gtk::Label());

    Gtk::Button *new_file = Gtk::manage(new Gtk::Button());

    en = Gtk::manage(new Gtk::Entry());
    new_file->set_image_from_icon_name("document-new");
    Gdk::RGBA black_back;

    black_back.set("#121411");
    
    new_file->signal_clicked().connect( sigc::mem_fun(*this,&hyp::HypWindow::insert_tab) );

    new_file->set_size_request(10,10);
    toolbar.set_size_request(-1,27);
    toolbar.override_background_color(black_back);
    toolbar.pack_start(*new_file,false,false,10);
    toolbar.pack_start(*status,false,false,10);

    toolbar.pack_end(*file,false,false,10);

    grand_window.override_background_color(black_backk);
    grand_window.pack_end(toolbar, false,false,0);

    // middlewindow to main window
    grand_window.add(middle_window);
    // main window to Final Window
    add(grand_window);
    //  END  ############
    std::cout<<"~~~~~~~~| Hyper Text |~~~~~~~~"<<std::endl;
}

//###############################################################################
/*
 *
 *
 *
 */ 
void hyp::HypWindow::insert_tab(){

    std::cout<<"\033[A\33[2K\r"<<"Inserting Tab -> Empty"<<std::endl;

    Gtk::Box *box = Gtk::manage(new Gtk::Box());
    Gtk::Label *d = Gtk::manage(new Gtk::Label("untitled"));
    Gtk::Button *but = Gtk::manage(new Gtk::Button());
    vec_scroll.push_back(Gtk::ScrolledWindow());

    but->set_image_from_icon_name("window-close");
    but->set_relief(Gtk::RELIEF_NONE);
    box->pack_start(*d,true,true,2);
    box->pack_end(*but,false,false,0);
    box->show_all();
   
    vec_text.push_back(hyp::HypTextView("untitled","",d,count));
    but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_text[count].path));

    vec_scroll[count].add(vec_text[count-icount]);

    vec_text[count-icount].set_show_line_numbers(true);
    vec_text[count-icount].set_monospace(true);

    but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_text[count-icount].path));
    // nb.append_page(vec_scroll[count-icount],*box);

    nb.append_page(vec_scroll[count],*box); 

   

    (*text_track)[count]=(count-icount);
    (*types)[count]="text";

    tracker.insert(count);
    count+=1;

    show_all();
}

//###############################################################################
/*
 *
 *
 *
 */ 
void hyp::HypWindow::on_tab_closed(int c,std::string path){
    std::cout<<"\033[A\33[2K\r"<<"Closing Tab : ";
    auto itrs = tracker.begin();
    auto itre = tracker.find(c);
    std::cout<<std::distance(itrs,itre)<<std::endl;
    nb.remove_page((int)std::distance(itrs,itre));
    if (path==""){

    }else{
        treeView->selected->erase(path);
    }
    tracker.erase(c);
    show_all();

}

//###############################################################################
/*
 *
 *
 *
 */ 
void hyp::HypWindow::on_file_open(){
    
    std::cout<<"\033[A\33[2K\r"<<"Opening File (Imported) : ";
    auto dialog = Gtk::FileChooserNative::create("Please choose a file",*this,Gtk::FILE_CHOOSER_ACTION_OPEN,"Choose","Cancel");
    dialog->run();
    std::cout<<dialog->get_filename()<<std::endl;

    std::string file = dialog->get_filename();

    Gtk::Box *box = Gtk::manage(new Gtk::Box());
    Gtk::Label *d = Gtk::manage(new Gtk::Label(std::filesystem::path(file).filename().string()));
    Gtk::Button *but = Gtk::manage(new Gtk::Button());
    vec_scroll.push_back(Gtk::ScrolledWindow());

    but->set_image_from_icon_name("window-close");
    but->set_relief(Gtk::RELIEF_NONE);
    box->pack_start(*d,true,true,2);
    box->pack_end(*but,false,false,0);
    box->show_all();

    if(std::filesystem::path(file).filename().extension().string()==".png" or std::filesystem::path(file).filename().extension().string()==".jpg" or std::filesystem::path(file).filename().extension().string()==".jpeg"  or std::filesystem::path(file).filename().extension().string()==".gif"){
        
        vec_imge.push_back(hyp::HypImgView(std::filesystem::path(file).filename(),file,count));        
        but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_imge[icount].path));
        vec_scroll[count].add(vec_imge[icount]);

        nb.append_page(vec_scroll[count],*box);
        (*types)[count]="image";
        (*img_track)[count]=icount;
        icount+=1;

    }else{

        vec_text.push_back(hyp::HypTextView(std::filesystem::path(file).filename(),file,d,count));
        vec_scroll[count].add(vec_text[count-icount]);
        vec_text[count-icount].set_show_line_numbers(true);
        vec_text[count-icount].set_monospace(true);
        but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_text[count-icount].path));
        // nb.append_page(vec_scroll[count-icount],*box);
        nb.append_page(vec_scroll[count],*box); 
        (*text_track)[count]=(count-icount);

        (*types)[count]="text";
    }
    
    // (vec_text[count].buffer)->set_text(Glib::file_get_contents(file));
    // (vec_text[count].buffer)->set_modified();

    tracker.insert(count);
    count+=1;

    show_all();
    
}
//###############################################################################
/* activates when the tree column is clicked
 *
 *
 */ 
void hyp::HypWindow::on_file_select(Glib::ustring fu){
    std::cout<<"\033[A\33[2K\r"<<"Opening File (Selected): ";    
    std::cout<<fu<<std::endl;

    std::string file = fu.c_str();

    Gtk::Box *box = Gtk::manage(new Gtk::Box());
    Gtk::Label *d = Gtk::manage(new Gtk::Label(std::filesystem::path(file).filename().string()));
    Gtk::Button *but = Gtk::manage(new Gtk::Button());
    vec_scroll.push_back(Gtk::ScrolledWindow());

    but->set_image_from_icon_name("window-close");
    but->set_relief(Gtk::RELIEF_NONE);
    box->pack_start(*d,true,true,2);
    box->pack_end(*but,false,false,0);
    box->show_all();

    if(std::filesystem::path(file).filename().extension().string()==".png" or std::filesystem::path(file).filename().extension().string()==".jpg" or std::filesystem::path(file).filename().extension().string()==".jpeg"  or std::filesystem::path(file).filename().extension().string()==".gif"){
        
        vec_imge.push_back(hyp::HypImgView(std::filesystem::path(file).filename(),file,count));        
        but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_imge[icount].path));
        vec_scroll[count].add(vec_imge[icount]);

        nb.append_page(vec_scroll[count],*box);
        (*types)[count]="image";
        (*img_track)[count]=icount;
        icount+=1;

    }else{

        vec_text.push_back(hyp::HypTextView(std::filesystem::path(file).filename(),file,d,count));
        vec_scroll[count].add(vec_text[count-icount]);
        vec_text[count-icount].set_show_line_numbers(true);
        vec_text[count-icount].set_monospace(true);
        but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_text[count-icount].path));
        // nb.append_page(vec_scroll[count-icount],*box);
        nb.append_page(vec_scroll[count],*box); 
        (*text_track)[count]=(count-icount);

        (*types)[count]="text";
    }
    
    tracker.insert(count);
    count+=1;

    show_all();
    
}
//###############################################################################


void hyp::HypWindow::on_thread_call(){

    if(thr and thr->joinable()){
        thr->join();
        status->set_label("Done");
    }
}

//###############################################################################
/*
 *
 *
 *
 */ 
void hyp::HypWindow::on_save(){

    auto itrs = tracker.begin();
    auto value=0;

    for (std::set<int>::iterator i = tracker.begin(); i != tracker.end(); ++i){
        if(std::distance(itrs,i)==nb.get_current_page()){
            value = *i;
            break;
        }
    }
    if ((*types)[value] != "image"){
        if (vec_text[(*text_track)[value]].path == ""){

        start:

            auto dialog = Gtk::FileChooserNative::create("Save New File",*this,Gtk::FILE_CHOOSER_ACTION_SAVE,"Save","Cancel");
            dialog->run();

            if (dialog->get_filename() == ""){ goto end; }

            if(Glib::file_test(dialog->get_filename(), Glib::FileTest::FILE_TEST_EXISTS)){
                Gtk::MessageDialog dialog(*this, "File Already Exists");
                dialog.set_secondary_text("The file name you entered is currently present in the directory.");
                dialog.run();
                goto start;
            }
            vec_text[(*text_track)[value]].path = dialog->get_filename();
            vec_text[(*text_track)[value]].file_name = std::filesystem::path(dialog->get_filename()).filename().string();
        end:
            std::cout<<"";
        }
        if(vec_text[(*text_track)[value]].path != ""){
            std::cout<<"Saving ~> "<<vec_text[(*text_track)[value]].path<<std::endl;
            Glib::file_set_contents(vec_text[(*text_track)[value]].path,vec_text[(*text_track)[value]].get_buffer()->get_text());
            vec_text[(*text_track)[value]].l->set_label(vec_text[(*text_track)[value]].file_name);
        }
        

    }

}
//###############################################################################
/*
 *
 *
 *
 */ 
void hyp::HypWindow::on_save_as(){

    start:

    auto dialog = Gtk::FileChooserNative::create("Save As",*this,Gtk::FILE_CHOOSER_ACTION_SAVE,"Save","Cancel");
    dialog->run();
    if (dialog->get_filename() == ""){ goto end; }

    if(Glib::file_test(dialog->get_filename(), Glib::FileTest::FILE_TEST_EXISTS)){
        Gtk::MessageDialog dialog(*this, "File Already Exists");
        dialog.set_secondary_text("The file name you entered is currently present in the directory.");
        dialog.run();
    }else{
        auto itrs = tracker.begin();
        auto value=0;

        for (std::set<int>::iterator i = tracker.begin(); i != tracker.end(); ++i){
            if(std::distance(itrs,i)==nb.get_current_page()){
                value = *i;
                break;
            }
        }
        if ((*types)[value] != "image"){
            vec_text[(*text_track)[value]].path = dialog->get_filename();
            vec_text[(*text_track)[value]].file_name = std::filesystem::path(dialog->get_filename()).filename().string();

            std::cout<<"Saving as ~> "<<vec_text[(*text_track)[value]].path<<std::endl;
            Glib::file_set_contents(vec_text[(*text_track)[value]].path,vec_text[(*text_track)[value]].get_buffer()->get_text());
            vec_text[(*text_track)[value]].l->set_label(vec_text[(*text_track)[value]].file_name);
        }

    }
    end:
        std::cout<<"";
}

//############################################################################### 
/* the function come in action when the "Open Folder" is clicked 
 *
 *
 */
void hyp::HypWindow::on_folder_open(){

    std::cout<<"\033[A\33[2K\r"<<"Opening Folder : ";
    auto dialog = Gtk::FileChooserNative::create("Select a folder",*this,Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER,"Open","Cancel");
    dialog->run();
    std::cout<<dialog->get_filename()<<std::endl;
    std::string x = dialog->get_filename();

    if (x==""){
        
    }else{
        status->set_label(std::string("Adding Folder - ")+x);
        
        thr = new std::thread([=,this]{treeView->add_folder(x,this);});
        middle_window.set_position(250);
        show_all();
    }
}


//############################################################################### 
/* Manages the File Type Label at the Bottom of the window using the Logic ...
 *    
 *
 */
void hyp::HypWindow::on_tab_change(Gtk::Widget* page, guint page_number){
    auto itrs = tracker.begin();
    // auto itre = tracker.find(page_number);
    auto value=0;
    for (std::set<int>::iterator i = tracker.begin(); i != tracker.end(); ++i){
        if(std::distance(itrs,i)==page_number){
            value = *i;
            break;
        }
    }
    if(nb.get_current_page()==page_number){
        if((*types)[value]=="image"){
            file->set_label(vec_imge[(*img_track)[value]].file_type);

        }else{
            file->set_label(vec_text[(*text_track)[value]].file_type);
        }
    }
}


//############################################################################### 
// THE Destructor:
hyp::HypWindow::~HypWindow(){
    std::cout<<"\x1b[37mHyperText \x1b[35mQuit\x1b[0m ... Bye !"<<std::endl;
}

//############################################################################### 

void hyp::HypWindow::on_menu_toggle(){
    bool active = false;
    m_refToggle->get_state(active);
    //The toggle action's state does not change automatically:
    active = !active;
    m_refToggle->change_state(active);

    Glib::ustring message;
    if (active)
        message = "Toggle is active.";
    else
        message = "Toggle is not active.";

    std::cout << message << std::endl;
}

//########################### Ok Enough For Now ############################################# 
