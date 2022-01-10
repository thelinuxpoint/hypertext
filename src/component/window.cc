#include <gtksourceviewmm.h>
#include <gtkmm.h>
#include <vector>
#include <iostream>
#include <functional>
#include <set>
#include <iterator>
#include <filesystem>
#include <thread>
#include <unistd.h>
#include <map>
#include "../header/window.h"

hyp::HypWindow::HypWindow(): Gtk::ApplicationWindow(), m_Box(Gtk::ORIENTATION_VERTICAL){
    
    set_title("Hyper Text");
    set_default_size(900, 600);

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


    Gtk::ScrolledWindow *for_tree = new Gtk::ScrolledWindow();

    m_refTreeModel = Gtk::TreeStore::create(m_Columns);

    m_TreeView = Gtk::manage(new Gtk::TreeView (m_refTreeModel));
    
    m_TreeView->set_model(m_refTreeModel);
    // m_TreeView->set_hover_selection(true);
    m_TreeView->set_headers_clickable(true);
    // m_TreeView->set_reorderable(true);

    m_tree_selector = m_TreeView->get_selection();
    m_tree_selector->set_select_function(sigc::mem_fun(*this, &hyp::HypWindow::on_row_select));


    hpy_column = new Gtk::TreeViewColumn();
    cell_pix = new Gtk::CellRendererPixbuf();
    cell_txt = new Gtk::CellRendererText();
    folders = new std::map<std::string,std::string>();

    cell_pix->set_property("pixbuf-expander-open", Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/open0.svg")));
    cell_pix->set_property("pixbuf-expander-closed", Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/close0.svg")));

    cell_pix->set_padding(3,1);

    hpy_column->pack_start(*cell_pix,false);
    hpy_column->pack_start(*cell_txt, true);

    hpy_column->add_attribute(*cell_txt,"text", 0);
    hpy_column->add_attribute(*cell_pix,"pixbuf", 1);

    hpy_column->set_title(" Folders");

    int x = m_TreeView->append_column(*hpy_column);

    m_TreeView->get_column(x-1)->add_attribute(*cell_pix,"stock-id",m_Columns.m_col_name);


    // ADD #####################################################################
    // treeview to scrollbar
    for_tree->add(*m_TreeView);
    //scrollbar to vbox
    tree.add(*for_tree);
    //vbox to middle window
    middle_window.add1(tree);
    // notebook to middle window
    middle_window.add2(nb);
    //##########################################################################

    // Edit menu ################################################################
    add_action("copy", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));

    add_action("openfolder", sigc::mem_fun(*this, &hyp::HypWindow::on_folder_open));

    add_action("openfile", sigc::mem_fun(*this, &hyp::HypWindow::on_file_open));
    
    add_action("newfile", sigc::mem_fun(*this, &hyp::HypWindow::insert_tab));
    //###########################################################################

    //Choices menus, to demonstrate Radio items,
    //using our convenience methods for string and int radio values:
    m_refChoice = add_action_radio_string("choice",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_choices), "a");

    m_refChoiceOther = add_action_radio_integer("choiceother",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_choices_other), 1);

    m_refToggle = add_action_bool("sometoggle",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_toggle), false);
    //Help menu:
    add_action("about", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));
    
    //##########################################################################
    //Create the toolbar and add it to a container widget:
    //##########################################################################
    m_refBuilder = Gtk::Builder::create();
    Glib::ustring ui_info =
    "<!-- Generated with glade 3.18.3 -->"
    "<interface>"
    "  <requires lib='gtk+' version='3.4'/>"
    "  <object class='GtkToolbar' id='toolbar'>"
    "    <property name='visible'>True</property>"
    "    <property name='can_focus'>False</property>"
    "    <child>"
    "      <object class='GtkToolButton' id='toolbutton_new'>"
    "        <property name='visible'>True</property>"
    "        <property name='can_focus'>False</property>"
    "        <property name='tooltip_text' translatable='yes'>New File</property>"
    "        <property name='action_name'>win.newfile</property>"
    "        <property name='icon_name'>document-new</property>"
    "      </object>"
    "      <packing>"
    "        <property name='expand'>False</property>"
    "        <property name='homogeneous'>True</property>"
    "      </packing>"
    "    </child>"
    "    <child>"
    "      <object class='GtkToolButton' id='toolbutton_quit'>"
    "        <property name='visible'>True</property>"
    "        <property name='can_focus'>False</property>"
    "        <property name='tooltip_text' translatable='yes'>Quit</property>"
    "        <property name='action_name'>app.quit</property>"
    "        <property name='icon_name'>application-exit</property>"
    "      </object>"
    "      <packing>"
    "        <property name='expand'>False</property>"
    "        <property name='homogeneous'>True</property>"
    "      </packing>"
    "    </child>"
    "  </object>"
    "</interface>";
    //##########################################################################

    try{
        m_refBuilder->add_from_string(ui_info);
    }
    catch (const Glib::Error& ex){
        std::cerr << "Building toolbar failed: " <<  ex.what();
    }
    Gtk::Toolbar* toolbar = nullptr;
    m_refBuilder->get_widget("toolbar", toolbar);

    if (!toolbar)
        g_warning("GtkToolbar not found");
    else
        grand_window.pack_end(*toolbar, false,false,0);
  
    // middlewindow to main window
    grand_window.add(middle_window);
    // main window to Final Window
    add(grand_window);
    //  END  ####################################################################
}

//###############################################################################

void hyp::HypWindow::insert_tab(){

    std::cout<<"Inserting Tab -> Empty"<<std::endl;
    vec_text.push_back(hyp::HypTextView("untitled","",count));

    vec_scroll.push_back(Gtk::ScrolledWindow());
    vec_scroll[count].add(vec_text[count]);
    //
    Gtk::Box *box = new Gtk::Box();
    Gtk::Button *but = new Gtk::Button();
    Gtk::Label *d = new Gtk::Label(vec_text[count].file_name);

    but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count));
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

void hyp::HypWindow::on_tab_closed(int c){
    std::cout<<"Closing Tab : ";
    auto itrs = tracker.begin();
    auto itre = tracker.find(c);
    std::cout<<std::distance(itrs,itre)<<std::endl;
    nb.remove_page((int)std::distance(itrs,itre));
    tracker.erase(c);
    show_all();

}

//###############################################################################

void hyp::HypWindow::on_file_open(){

    std::cout<<"Inserting Tab -> Filled"<<std::endl;
    std::cout<<"Opening File : ";
    auto dialog = Gtk::FileChooserNative::create("Please choose a file",*this,Gtk::FILE_CHOOSER_ACTION_OPEN,"Choose","Cancel");
    dialog->run();
    std::cout<<dialog->get_filename()<<std::endl;


    vec_text.push_back(hyp::HypTextView(std::filesystem::path(dialog->get_filename()).filename(),dialog->get_filename(),count));
    vec_scroll.push_back(Gtk::ScrolledWindow());
    (vec_text[count].get_buffer())->set_text(Glib::file_get_contents(dialog->get_filename()));


    vec_scroll[count].add(vec_text[count]);
    //
    Gtk::Box *box = new Gtk::Box();
    Gtk::Button *but = new Gtk::Button();
    Gtk::Label *d = new Gtk::Label(vec_text[count].file_name);

    but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count));
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
void hyp::HypWindow::on_file_select(std::string file){
    std::cout<<"Inserting Tab -> Filled"<<std::endl;
    std::cout<<"Opening File : ";
    
    std::cout<<file<<std::endl;


    vec_text.push_back(hyp::HypTextView(std::filesystem::path(file).filename(),file,count));
    vec_scroll.push_back(Gtk::ScrolledWindow());
    (vec_text[count].get_buffer())->set_text(Glib::file_get_contents(file));


    vec_scroll[count].add(vec_text[count]);
    //
    Gtk::Box *box = new Gtk::Box();
    Gtk::Button *but = new Gtk::Button();
    Gtk::Label *d = new Gtk::Label(vec_text[count].file_name);

    but->signal_clicked().connect( sigc::bind(sigc::mem_fun(*this,&hyp::HypWindow::on_tab_closed),count));
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
bool hyp::HypWindow::on_row_select(const Glib::RefPtr<Gtk::TreeModel>& b,const Gtk::TreeModel::Path& c,bool a){

    // std::cout<<"Selected ... "<<std::flush<<c.to_string()<<std::endl;
    if(std::filesystem::is_regular_file((*folders)[c.to_string()])){
        on_file_select((*folders)[c.to_string()]);
        return false;
    }
    return false;
}

//###############################################################################

void hyp::HypWindow::on_folder_open(){
 
 std::cout<<"Opening Folder : ";
    std::string x;
    {
        auto dialog = Gtk::FileChooserNative::create("Select a folder",*this,Gtk::FILE_CHOOSER_ACTION_SELECT_FOLDER,"Open","Cancel");
        dialog->run();
        std::cout<<dialog->get_filename()<<std::endl;
        x=dialog->get_filename();
    }
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    
    set_dir(x,row,std::to_string(m_row));
    m_row++;
}

//############################################################################### 

void hyp::HypWindow::set_dir(std::string fold,Gtk::TreeModel::Row &row,std::string x){

    row[m_Columns.m_col_name] = std::filesystem::path(fold).filename().string();
    x=x+":";
    int m_child=0;
    for(auto const& dir_entry: std::filesystem::directory_iterator{std::filesystem::path(fold)}){
        

        if (Glib::file_test(dir_entry.path().string(),Glib::FILE_TEST_IS_DIR)){
            // std::cout<<"found dir : "<<dir_entry.path().string()<<std::endl;
            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));

            set_dir(dir_entry.path().string(),childrow,x+std::to_string(m_child));
            m_child++;
        }else{

            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
            // FILE TYPES 
            if (dir_entry.path().filename().extension().string() == ".cc" or dir_entry.path().filename().extension().string() == ".cpp" ){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/cpp/cpp.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".md"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/md/md.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".o"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/exe.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".svg"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/xml/xml.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".py"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/python/python.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".rb"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/ruby/ruby.svg"),22,22 );
            }else if(dir_entry.path().filename().extension().string() == ".js"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/js/js.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".css"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/css3/css3.svg"),24,24 );
            }else if(dir_entry.path().filename().extension().string() == ".json"){
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/json/json.svg"),24,24 );
            }else{
                childrow[m_Columns.m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/text2.svg"),24,24 );
            }
            childrow[m_Columns.m_col_name] = dir_entry.path().filename().string();
            (*folders)[(x+std::to_string(m_child))] = dir_entry.path().string();
            // std::cout<<(x+std::to_string(m_child))<<std::endl;
            m_child++;
        }

    }
}

//////////////////////////////////

hyp::HypWindow::~HypWindow(){}


void hyp::HypWindow::on_menu_others(){
    std::cout << "A menu item was selected." << std::endl;
}




























/////////////////////////////////
void hyp::HypWindow::on_menu_choices(const Glib::ustring& parameter){
  //The radio action's state does not change automatically:
    m_refChoice->change_state(parameter);

    Glib::ustring message;
    if (parameter == "a")
        message = "Choice a was selected.";
    else
        message = "Choice b was selected.";
    std::cout << message << std::endl;
}
/////////////////////////////////////
void hyp::HypWindow::on_menu_choices_other(int parameter){
    //The radio action's state does not change automatically:
    m_refChoiceOther->change_state(parameter);

    Glib::ustring message;
    if (parameter == 1)
        message = "Choice 1 was selected.";
    else
        message = "Choice 2 was selected.";
    std::cout << message << std::endl;
}

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
