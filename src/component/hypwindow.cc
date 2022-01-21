#include "../header/hypwindow.h"

/* The Constructor:
 *
 *
 *
 *
 */ 
hyp::HypWindow::HypWindow(): Gtk::ApplicationWindow(), m_Box(Gtk::ORIENTATION_VERTICAL){
    
    set_title("Hyper Text");
    set_default_size(900, 600);

    Gdk::RGBA black_backk;
    black_backk.set("#151515");
    override_background_color(black_backk);
    set_default_icon(Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/512.png"),511,511));
    
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

    for_tree = new Gtk::ScrolledWindow();

    m_Columns = new ModelColumns();


    m_refTreeModel = Gtk::TreeStore::create(*m_Columns);

    m_TreeView = Gtk::manage(new Gtk::TreeView (m_refTreeModel));
    
    m_TreeView->set_model(m_refTreeModel);
 
    m_TreeView->override_background_color(black_backk);

    m_tree_selector = m_TreeView->get_selection();
    m_tree_selector->set_select_function(sigc::mem_fun(*this, &hyp::HypWindow::on_row_select));
    
    m_refTreeModel->signal_row_inserted().connect( sigc::mem_fun(*this,&hyp::HypWindow::on_row_insert));

    hpy_column = Gtk::manage(new Gtk::TreeViewColumn());
    cell_pix = Gtk::manage(new Gtk::CellRendererPixbuf());
    cell_txt = Gtk::manage(new Gtk::CellRendererText());

    folders = new std::map<std::string,std::string>();
    selected = new std::set<std::string>();
    types = new std::map<int,std::string>();
    text_track = new std::map<int,int>();
    img_track = new std::map<int,int>();

    hyp_dispatch.connect(sigc::mem_fun(*this,&hyp::HypWindow::on_thread_call));

    cell_pix->set_property("pixbuf-expander-open", Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/open0.svg")));
    cell_pix->set_property("pixbuf-expander-closed", Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/close0.svg")));
    cell_pix->set_padding(3,1);
    
    hpy_column->pack_start(*cell_pix,false);
    hpy_column->pack_start(*cell_txt, true);
    hpy_column->add_attribute(*cell_txt,"text", 0);
    hpy_column->add_attribute(*cell_pix,"pixbuf", 1);
    hpy_column->set_reorderable();
    hpy_column->set_title(" Folders");
    // hpy_column->set_resizable();


    m_TreeView->signal_row_expanded().connect( sigc::mem_fun(*this,&hyp::HypWindow::on_tree_click));
    m_TreeView->signal_row_collapsed().connect( sigc::mem_fun(*this,&hyp::HypWindow::on_tree_click));  

    m_TreeView->set_vexpand();
    // m_TreeView->set_visible(false);
    m_TreeView->set_enable_tree_lines( true);

    // hyp::HypImgView("/home/prakash/bitmap.png");
    // nb.append_page(hyp::HypImgView("/home/prakash/bitmap.png"));
    
    int x = m_TreeView->append_column(*hpy_column);
    m_TreeView->get_column(x-1)->add_attribute(*cell_pix,"stock-id",m_Columns->m_col_name);

    nb.override_background_color(black_backk);
    nb.signal_switch_page().connect( sigc::mem_fun(*this,&hyp::HypWindow::on_tab_change));
    //##########################################################################    
    // ADD #####################################################################

    // treeview to scrollbar
    for_tree->add(*m_TreeView);
    for_tree->set_vexpand();
    //scrollbar to vbox
    // tree.add(*for_tree);
    //vbox to middle window
    middle_window.add1(*for_tree);
    // notebook to middle window
    middle_window.add2(nb);

    middle_window.set_position(0);

    //##########################################################################
    // Edit menu ################################################################

    // add_action("copy", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));

    add_action("openfolder", sigc::mem_fun(*this, &hyp::HypWindow::on_folder_open));

    add_action("openfile", sigc::mem_fun(*this, &hyp::HypWindow::on_file_open));
    
    add_action("newfile", sigc::mem_fun(*this, &hyp::HypWindow::insert_tab));

    add_action("save", sigc::mem_fun(*this, &hyp::HypWindow::on_save));


    //###########################################################################

    //Choices menus, to demonstrate Radio items,
    //using our convenience methods for string and int radio values:
    m_refToggle = add_action_bool("sometoggle",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_toggle), false);
    //Help menu:
    // add_action("about", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));
    
    //##########################################################################
    //Create the toolbar and add it to a container widget:
    //##########################################################################
    file = new Gtk::Label("Plain Text");
    status = new Gtk::Label();

    Gtk::Button new_file;
    new_file.set_image_from_icon_name("document-new");
    Gdk::RGBA black_back;

    black_back.set("#121411");
    toolbar.set_size_request(-1,25);
    toolbar.override_background_color(black_back);

    toolbar.pack_start(new_file,false,false,10);;
    toolbar.pack_start(*status,false,false,10);
    toolbar.pack_end(*file,false,false,10);

    grand_window.override_background_color(black_backk);
    grand_window.pack_end(toolbar, false,false,0);

    // middlewindow to main window
    grand_window.add(middle_window);
    // main window to Final Window
    add(grand_window);
    //  END  ############
}

//###############################################################################
/*
 *
 *
 *
 */ 
void hyp::HypWindow::insert_tab(){

    std::cout<<"Inserting Tab -> Empty"<<std::endl;
    Gtk::Label *d = new Gtk::Label("untitled");
    vec_text.push_back(hyp::HypTextView("untitled","",d,count));

    vec_scroll.push_back(Gtk::ScrolledWindow());
    vec_scroll[count].add(vec_text[count]);
    //
    Gtk::Box *box = new Gtk::Box();
    Gtk::Button *but = new Gtk::Button();

    but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_text[count].path));
    vec_text[count].set_show_line_numbers(true);
    vec_text[count].set_monospace(true);
    

    but->set_image_from_icon_name("window-close");
    but->set_relief(Gtk::RELIEF_NONE);
    box->pack_start(*d,true,true,2);
    box->pack_end(*but,false,false,0);
    box->show_all();

    nb.append_page(vec_scroll[count],*box);

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
    std::cout<<"Closing Tab : ";
    auto itrs = tracker.begin();
    auto itre = tracker.find(c);
    std::cout<<std::distance(itrs,itre)<<std::endl;
    nb.remove_page((int)std::distance(itrs,itre));
    if (path==""){

    }else{
        selected->erase(path);
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
    
    std::cout<<"------------------------"<<std::endl;

    std::cout<<"Inserting Tab -> Filled"<<std::endl;
    std::cout<<"Opening File : ";
    auto dialog = Gtk::FileChooserNative::create("Please choose a file",*this,Gtk::FILE_CHOOSER_ACTION_OPEN,"Choose","Cancel");
    dialog->run();
    std::cout<<dialog->get_filename()<<std::endl;
    std::cout<<"------------------------"<<std::endl;


    Gtk::Label *d = Gtk::manage(new Gtk::Label(std::filesystem::path(dialog->get_filename()).filename().string()));
    vec_text.push_back(hyp::HypTextView(std::filesystem::path(dialog->get_filename()).filename(),dialog->get_filename(),d,count));
    vec_scroll.push_back(Gtk::ScrolledWindow());
    // (vec_text[count].buffer)->set_text(Glib::file_get_contents(dialog->get_filename()));


    vec_scroll[count].add(vec_text[count]);
    //
    Gtk::Box *box = new Gtk::Box();
    Gtk::Button *but = new Gtk::Button();

    but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count,vec_text[count].path));
    vec_text[count].set_show_line_numbers(true);
    vec_text[count].set_monospace(true);
    

    but->set_image_from_icon_name("window-close");
    but->set_relief(Gtk::RELIEF_NONE);
    box->pack_start(*d,true,true,2);
    box->pack_end(*but,false,false,0);
    box->show_all();

    nb.append_page(vec_scroll[count],*box);

    tracker.insert(count);
    count+=1;

    show_all();

}
//###############################################################################
/* activates when the tree column is clicked
 *
 *
 */ 
void hyp::HypWindow::on_file_select(std::string file){
    std::cout<<"------------------------"<<std::endl;
    std::cout<<"Inserting Tab from Sidebar -> Filled "<<std::endl;
    std::cout<<"Opening File : ";    
    std::cout<<file<<std::endl;
    std::cout<<"------------------------"<<std::endl;


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
bool hyp::HypWindow::on_row_select(const Glib::RefPtr<Gtk::TreeModel>& b,const Gtk::TreeModel::Path& c,bool a){
    // for Debuging only:
    // std::cout<<"Selected ... "<<std::flush<<c.to_string()<<std::endl;
    show_all();
    if(std::filesystem::is_regular_file((*folders)[c.to_string()]) and (selected->count((*folders)[c.to_string()]) == 0) ){
        on_file_select((*folders)[c.to_string()]);

        file->set_label(file_type_analyze(std::filesystem::path((*folders)[c.to_string()]).filename().string()));
        
        selected->insert((*folders)[c.to_string()]);
        return false;
    }
    return false;
}

//############################################################################### 
// columns are overlapping and i cant find solution for it
// so we will deal with it ... later on ...
void hyp::HypWindow::on_tree_click(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path){
    
    m_TreeView->columns_autosize();
    for_tree->queue_resize();
    m_TreeView->queue_resize();
    m_TreeView->show_all();

}

void hyp::HypWindow::on_row_insert(const Gtk::TreeModel::Path& path,const Gtk::TreeModel::iterator& iter){
    show_all();
}
//###############################################################################

void hyp::HypWindow::on_thread_call(){
    
   

    if(thr and thr->joinable()){
        thr->join();
        status->set_label("Done");
    }
    
    // m_TreeView->set_sensitive(true);
    // thr = nullptr;
}
//###############################################################################
void hyp::HypWindow::notify(){
    hyp_dispatch.emit();
}

//###############################################################################
/*
 *
 *
 *
 */ 
void hyp::HypWindow::on_save(){
    // std::cout<<nb.get_current_page()<<std::endl;
    auto itrs = tracker.begin();
    // auto itre = tracker.find(page_number);
    auto value=0;

    for (std::set<int>::iterator i = tracker.begin(); i != tracker.end(); ++i){
        if(std::distance(itrs,i)==nb.get_current_page()){
            value = *i;
            break;
        }
    }
    if ((*types)[value] != "image"){
        std::cout<<"Saving ~> "<<vec_text[(*text_track)[value]].path<<std::endl;
        Glib::file_set_contents(vec_text[(*text_track)[value]].path,vec_text[(*text_track)[value]].get_buffer()->get_text());
        vec_text[(*text_track)[value]].l->set_label(vec_text[(*text_track)[value]].file_name);
        // vec_text[(*text_track)[value]].get_buffer().get_text();

    }

}
//############################################################################### 
/* the function come in action when the "Open Folder" is clicked 
 *
 *
 */
void hyp::HypWindow::on_folder_open(){

    std::cout<<"Opening Folder : ";
    auto dialog = Gtk::FileChooserNative::create("Select a folder",*this,Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER,"Open","Cancel");
    dialog->run();
    std::cout<<dialog->get_filename()<<std::endl;
    std::string x = dialog->get_filename();

    if (x==""){
        
    }else{
        Gtk::TreeModel::Row row = *(m_refTreeModel->append());
        status->set_label(std::string("Adding Folder - ")+x);
    
        // the fast method and errorfull method    
        // for_tree->set_sensitive(false);
        // m_TreeView->set_sensitive(false);

        thr = Glib::Thread::create(sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::set_dir),x,row,std::to_string(m_row)));

        
        // auto c = Glib::ThreadPool(10,true);
        // c.push(sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::set_dir),x,row,std::to_string(m_row)));

        // the slow method
        // set_dir(x,row,std::to_string(m_row));
        m_row++;
        middle_window.set_position(250);
        show_all();
    }
}

//############################################################################### 
/* The Function used to set the folder to the folder section of the window using recursion
 *
 *
 */
void hyp::HypWindow::set_dir(std::string fold,Gtk::TreeModel::Row &row,std::string x){
    row[m_Columns->m_col_name] = std::filesystem::path(fold).filename().string();
    x=x+":";
    int m_child=0;
    ++thr_count;
    
    for(auto const& dir_entry: std::filesystem::directory_iterator{std::filesystem::path(fold)}){

        if (Glib::file_test(dir_entry.path().string(),Glib::FILE_TEST_IS_DIR)){

            //std::cout<<"Directory: "<<dir_entry.path().string()<<std::endl;
            
            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));            
            
            set_dir(dir_entry.path().string(),childrow,x+std::to_string(m_child));
            m_child++;
            

        }else{

            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
            // FILE TYPES 
            if (dir_entry.path().filename().extension().string() == ".cc" or dir_entry.path().filename().extension().string() == ".cpp" ){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/cpp/cpp.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".c"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/c/c.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".md"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/md/md.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".o"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/exe.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".svg"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/xml/xml.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".py"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/python/python.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".php"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/php/php.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".rb"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/ruby/ruby.svg"),22,22 );
            }else if(dir_entry.path().filename().extension().string() == ".js"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/js/js.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".css"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/css3/css3.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".json"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/json/json.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".jpg" or dir_entry.path().filename().extension().string() == ".png" or dir_entry.path().filename().extension().string() == ".jpeg"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/drawing.svg"),24,24 );
            }else{
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/text2.svg"),24,24 );
            }
            mtx.lock();
            childrow[m_Columns->m_col_name] = dir_entry.path().filename().string();
            (*folders)[(x+std::to_string(m_child))] = dir_entry.path().string();
            mtx.unlock();
            // for Debuging only:
            // std::cout<<(x+std::to_string(m_child))<<std::endl;
            m_child++;        
        }
    }
    --thr_count;
    if(thr_count==0){ notify();}
    for_tree->show_all();    

    

}
//############################################################################### 
/* The File Type Analyzer for the hyp::HypWindow::on_row_select(...) function
 *
 */

std::string hyp::HypWindow::file_type_analyze(std::string file){
    if(std::filesystem::path(file).extension().string()==".cc" or std::filesystem::path(file).extension().string()==".cpp"){
        return std::string("C++");
    }else if(std::filesystem::path(file).extension().string()==".c"){
        return std::string("C");
    }else if(std::filesystem::path(file).extension().string()==".h"){
        return std::string("C/C++ header");
    }else if(std::filesystem::path(file).extension().string()==".py"){
        return std::string("Python");
    }else if(std::filesystem::path(file).extension().string()==".md"){
        return std::string("Markdown");
    }else if(std::filesystem::path(file).extension().string()==".js"){
        return std::string("Javascript");
    }else if(std::filesystem::path(file).extension().string()==".rs"){
        return std::string("Rust");
    }else if(std::filesystem::path(file).extension().string()==".java"){
        return std::string("Java");
    }else if(std::filesystem::path(file).extension().string()==".svg"){
        return std::string("XML");
    }else if(std::filesystem::path(file).extension().string()==".html"){
        return std::string("HTML");
    }else if(std::filesystem::path(file).extension().string()==".json"){
        return std::string("JSON");
    }else if(std::filesystem::path(file).extension().string()==".png"){
        return std::string("PNG");
    }else if(std::filesystem::path(file).extension().string()==".jpg"){
        return std::string("JPG");
    }else if(std::filesystem::path(file).extension().string()==".jpeg"){
        return std::string("JPEG");
    }else if(std::filesystem::path(file).extension().string()==".gif"){
        return std::string("GIF");
    }else if(std::filesystem::path(file).filename().string()=="Makefile" or std::filesystem::path(file).filename().string()=="MAKEFILE" ){
        return std::string("Makefile");
    }
    return std::string("Plain Text");
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
    std::cout<<"\x1b[36mHypWindow\x1b[0m ... Destructed"<<std::endl;
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
