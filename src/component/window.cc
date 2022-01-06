#include <gtksourceviewmm.h>
#include <gtkmm.h>
#include <vector>
#include <iostream>
#include <functional>
#include <set>
#include <iterator>
#include <filesystem>
#include <thread>
#include "../header/window.h"

hyp::HypWindow::HypWindow(): Gtk::ApplicationWindow(), m_Box(Gtk::ORIENTATION_VERTICAL){
    
    set_title("Hyper Text");
    set_default_size(700, 500);
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

    // tree.set_margin(5);
    Gtk::ScrolledWindow *for_tree = new Gtk::ScrolledWindow();
    m_refTreeModel = Gtk::TreeStore::create(m_Columns);

    m_TreeView = Gtk::manage(new Gtk::TreeView (m_refTreeModel));
    m_TreeView->set_hover_selection(true);
    m_TreeView->append_column("", m_Columns.m_col_pict);
    // m_TreeView->set_enable_tree_lines();
    m_TreeView->append_column("FOLDERS", m_Columns.m_col_name);

    for_tree->add(*m_TreeView);

    tree.add(*for_tree);

    middle_window.add1(tree);
    middle_window.add2(nb);


    // m_TreeView.set_model(m_refTreeModel);


    // Edit menu: ###############################################################
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

    //Create the toolbar and add it to a container widget:

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
  
    // m_TreeView.append_column("ID", m_Columns.m_col_id);


    grand_window.add(middle_window);

    add(grand_window);

    // show_all();
}
//////////////////////////////////////////////////////////////////

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
//////////////////////////////////

void hyp::HypWindow::on_tab_closed(int c){
    std::cout<<"Closing Tab : ";
    auto itrs = tracker.begin();
    auto itre = tracker.find(c);
    std::cout<<std::distance(itrs,itre)<<std::endl;
    nb.remove_page((int)std::distance(itrs,itre));
    tracker.erase(c);
    show_all();

}
///////////////////////////////////////////////////////////////////////////

void hyp::HypWindow::on_file_open(){
    std::cout<<"Opening File : ";
    auto dialog = Gtk::FileChooserNative::create("Please choose a file",*this,Gtk::FILE_CHOOSER_ACTION_OPEN,"Choose","Cancel");
    dialog->run();
    std::cout<<dialog->get_filename()<<std::endl;

    std::cout<<"Inserting Tab -> Filled"<<std::endl;


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

///////////////////////////////////////////////////////////////////////////
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

    set_dir(x,row);

}
// ////////////////////////////////////////////////

void hyp::HypWindow::set_dir(std::string fold,Gtk::TreeModel::Row &row){

    row[m_Columns.m_col_pict] = Gdk::Pixbuf::create_from_file("/home/prakash/Downloads/close0.svg",20,20);
    // row[m_Columns.m_col_id] = 1;
    row[m_Columns.m_col_name] = std::filesystem::path(fold).filename().string();

    for(auto const& dir_entry: std::filesystem::directory_iterator{std::filesystem::path(fold)}){
        

        if (Glib::file_test(dir_entry.path().string(),Glib::FILE_TEST_IS_DIR)){
            // std::cout<<"found dir : "<<dir_entry.path().string()<<std::endl;
            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));

            set_dir(dir_entry.path().string(),childrow);
        }else{

            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
            // childrow.set_expand(false);

            // std::cout<<"Appending : "<<dir_entry.path().filename().string()<<std::endl;
            childrow[m_Columns.m_col_pict] = Gdk::Pixbuf::create_from_file("/home/prakash/Downloads/aa.png",10,10);
            // childrow[m_Columns.m_col_id] = 1;//dir_entry.path().filename().string();

            childrow[m_Columns.m_col_name] = dir_entry.path().filename().string();

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
