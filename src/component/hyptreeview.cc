#include "../header/hypwindow.h"
/*[#] Constructor:
 *
 *
 */

#define hyplog(x) std::cout<<std::flush<<x;

hyp::HypTreeView::HypTreeView(hyp::HypWindow *parent){
	//
	Gdk::RGBA black_backk;black_backk.set("#151515");
	//
	m_Columns = new ModelColumns();
	//
    m_refTreeModel = Gtk::TreeStore::create(*m_Columns);

	sorted_model = Gtk::TreeModelSort::create(m_refTreeModel);

	sorted_model->set_sort_column(m_Columns->m_col_name, Gtk::SORT_ASCENDING);
	get_style_context()->add_class("txtview");
    // initialize the treeview constructor
    Gtk::TreeView();
    // set the tree model
    set_model(sorted_model);
	// set color for background
	override_background_color(black_backk);
	//
    str = new std::string();
    org = new std::string();


	set_vexpand();
	//
	// set_enable_tree_lines( true);
	//
	m_tree_selector = get_selection();

    m_tree_selector->set_select_function(sigc::bind(sigc::mem_fun(*this,&hyp::HypTreeView::on_row_select),parent));

	hpy_column = Gtk::manage(new Gtk::TreeViewColumn());
    //
    cell_pix = Gtk::manage(new Gtk::CellRendererPixbuf());
    //
    cell_txt = Gtk::manage(new Gtk::CellRendererText());

    //
    //


    selected = new std::set<std::string>();
	//
	cell_pix->set_property("pixbuf-expander-open", Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/open0.svg")));
    cell_pix->set_property("pixbuf-expander-closed", Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/close0.svg")));
    cell_pix->set_padding(3,1);

    //
    hpy_column->pack_start(*cell_pix,false);
    hpy_column->pack_start(*cell_txt, true);

    hpy_column->add_attribute(*cell_txt,"text", 0);
    hpy_column->add_attribute(*cell_pix,"pixbuf", 1);

    hpy_column->set_reorderable();
    hpy_column->set_title(" Folders");




    // Fill the menu1
    auto item2 = Gtk::make_managed<Gtk::MenuItem>("Rename Folder", true);
    item2->signal_activate().connect(sigc::mem_fun(*this, &hyp::HypTreeView::on_menu_file_rename) );
    m_Menu_1.append(*item2);

    item2 = Gtk::make_managed<Gtk::MenuItem>("Add File", true);
    item2->signal_activate().connect(sigc::mem_fun(*this, &hyp::HypTreeView::on_menu_file_addfile) );
    m_Menu_1.append(*item2);

    item2 = Gtk::make_managed<Gtk::MenuItem>("New Folder", true);
    item2->signal_activate().connect(sigc::mem_fun(*this, &hyp::HypTreeView::on_menu_file_addfolder) );
    m_Menu_1.append(*item2);

    item2 = Gtk::make_managed<Gtk::MenuItem>("Remove Folder From Project", true);
    item2->signal_activate().connect(sigc::mem_fun(*this, &hyp::HypTreeView::on_menu_file_removefolder));
    m_Menu_1.append(*item2);



    m_Menu_1.accelerate(*this);
    m_Menu_1.show_all(); //Show all menu items when the menu pops up

    // Fill the menu2

    auto item = Gtk::make_managed<Gtk::MenuItem>("Rename File", true);
    item->signal_activate().connect(sigc::mem_fun(*this, &hyp::HypTreeView::on_menu_file_rename) );
    m_Menu_2.append(*item);

    item = Gtk::make_managed<Gtk::MenuItem>("Delete File", true);
    item->signal_activate().connect(sigc::mem_fun(*this, &hyp::HypTreeView::on_menu_file_addfile) );
    m_Menu_2.append(*item);

    item = Gtk::make_managed<Gtk::MenuItem>("Open Containing Folder", true);
    item->signal_activate().connect(sigc::mem_fun(*this, &hyp::HypTreeView::on_menu_file_addfolder) );
    m_Menu_2.append(*item);


    m_Menu_2.accelerate(*this);
    m_Menu_2.show_all(); //Show all menu items when the menu pops up


    //###############################
	signal_row_expanded().connect( sigc::mem_fun(*this,&hyp::HypTreeView::on_tree_click));

    int x = append_column(*hpy_column);
    get_column(x-1)->add_attribute(*cell_pix,"stock-id",m_Columns->m_col_name);

}
/*[#] Opens File when column of TreeView is Clicked:
 *
 *
 *
 */
bool hyp::HypTreeView::on_row_select(const Glib::RefPtr<Gtk::TreeModel>& b,const Gtk::TreeModel::Path& cb,bool a,hyp::HypWindow *parent){
    // for Debuging only:
    // std::cout<<"Selected ... "<<std::flush<<c.to_string()<<std::endl;
    show_all();
    auto c = sorted_model->convert_path_to_child_path(cb);
    *str = c.to_string();
    *org = cb.to_string();

    auto it = m_refTreeModel->get_iter(*str);
    auto check_ext = std::filesystem::path(Glib::ustring((*it)[m_Columns->m_col_path]).raw()).filename().extension().string();

    //
    if(std::filesystem::is_regular_file(Glib::ustring((*it)[m_Columns->m_col_path]).raw()) and (selected->count(Glib::ustring((*it)[m_Columns->m_col_path]).raw()) == 0) ){
        if (check_ext == ".cc" or check_ext == ".cpp" ){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/cpp/cpp.svg"),24,24 );
        }else if(check_ext == ".c"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/c/c.svg"),24,24 );
        }else if(check_ext == ".md"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/md/md.svg"),24,24 );
        }else if(check_ext == ".o"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/exe.svg"),24,24 );
        }else if(check_ext == ".svg"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/xml/xml.svg"),24,24 );
        }else if(check_ext == ".py"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/python/python.svg"),24,24 );
        }else if(check_ext == ".php"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/php/php.svg"),24,24 );
        }else if(check_ext == ".rb"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/ruby/ruby.svg"),22,22 );
        }else if(check_ext == ".rs"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/rust/rust.svg"),22,22 );
        }else if(check_ext == ".js"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/js/js.svg"),24,24 );
        }else if(check_ext == ".css"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/css3/css3.svg"),24,24 );
        }else if(check_ext == ".json"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/json/json.svg"),24,24 );
        }else if(check_ext == ".jpg" or check_ext == ".png" or check_ext == ".jpeg"){
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/drawing.svg"),24,24 );
        }else{
            (*it)[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/text2.svg"),24,24 );
        }

        parent->on_file_select((*it)[m_Columns->m_col_path]);

        parent->file->set_label(file_type_analyze(std::filesystem::path(Glib::ustring((*it)[m_Columns->m_col_path]).raw()).filename().string()));
        selected->insert(Glib::ustring((*it)[m_Columns->m_col_path]).raw());
        return false;
    }else{

        if( row_expanded(cb) ) {
            collapse_row(cb);
        }else{
            expand_to_path(cb);
        }
    }

    return false;
}
/*[#] Add Folder Protocol:
 *
 *
 *
 */
void hyp::HypTreeView::add_folder(std::string path,hyp::HypWindow *caller){

	Gtk::TreeModel::Row row = *(m_refTreeModel->append());
	set_dir(path,row,std::to_string(m_row));
	m_row++;
    caller->hyp_dispatch.emit();
    show_all();
}
/*[#] Add Folder Main:
 *
 *
 *
 */
void hyp::HypTreeView::set_dir(std::string fold,Gtk::TreeModel::Row &row,std::string x){
    row[m_Columns->m_col_name] = std::filesystem::path(fold).filename().string();
    row[m_Columns->m_col_path] = std::filesystem::path(fold).string();

    x=x+":";
    int m_child=0;

    for(auto const& dir_entry: std::filesystem::directory_iterator{std::filesystem::path(fold)}){

        if (Glib::file_test(dir_entry.path().string(),Glib::FILE_TEST_IS_DIR)){

            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));

            set_dir(dir_entry.path().string(),childrow,x+std::to_string(m_child));

            m_child++;


        }else{

            Gtk::TreeModel::Row childrow = *(m_refTreeModel->append(row.children()));
            // FILE TYPES
            auto check_ext = dir_entry.path().filename().extension().string();

            if (check_ext == ".cc" or check_ext == ".cpp" ){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/cpp/cpp.svg"),24,24 );
            }else if(check_ext == ".c"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/c/c.svg"),24,24 );
            }else if(check_ext == ".md"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/md/md.svg"),24,24 );
            }else if(check_ext == ".svg"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/xml/xml.svg"),24,24 );
            }else if(check_ext == ".py"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/python/python.svg"),24,24 );
            }else if(check_ext == ".php"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/php/php.svg"),24,24 );
            }else if(check_ext == ".rb"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/ruby/ruby.svg"),22,22 );
            }else if(check_ext == ".rs"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/rust/rust.svg"),22,22 );
            }else if(check_ext == ".js"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/js/js.svg"),24,24 );
            }else if(check_ext == ".css"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/css3/css3.svg"),24,24 );
            }else if(check_ext == ".json"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/json/json.svg"),24,24 );
            }else if(check_ext == ".jpg" or check_ext == ".png" or check_ext == ".jpeg"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/drawing.svg"),24,24 );
            }else{
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/text2.svg"),24,24 );
            }

            childrow[m_Columns->m_col_name] = dir_entry.path().filename().string();
            childrow[m_Columns->m_col_path] = dir_entry.path().string();


            // for Debuging only:
            // std::cout<<(x+std::to_string(m_child))<<std::endl;
            m_child++;
        }
    }
   	show_all();
}

/*
 *
 *
 *
 */
std::string hyp::HypTreeView::file_type_analyze(std::string file){
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
/*
 *
 *
 *
 */
void hyp::HypTreeView::on_menu_file_removefolder(){
    auto tmp = m_refTreeModel->get_iter(*str);
    m_refTreeModel->erase(tmp);
    sorted_model = Gtk::TreeModelSort::create(m_refTreeModel);

    sorted_model->set_sort_column(m_Columns->m_col_name, Gtk::SORT_ASCENDING);
    unset_model();set_model(sorted_model);

}


void hyp::HypTreeView::on_menu_file_rename(){

    // std::cout << "Rename: " <<(*folders)[*str]<< std::endl;

}
/*
 *
 *
 *
 */
void hyp::HypTreeView::on_menu_file_addfolder(){

    Gtk::Dialog *dialog = new Gtk::Dialog("Add Folder Name");
    Gtk::Entry *entry = new Gtk::Entry();
    dialog->add_button("Cancel",Gtk::RESPONSE_NO);
    dialog->add_button("OK",Gtk::RESPONSE_YES);

    dialog->get_vbox()->pack_start(*entry,false,false,10);
    entry->set_size_request(200,40);
    dialog->set_size_request(300,80);
    dialog->show_all();


    int result = dialog->run();

    switch (result){
        case Gtk::RESPONSE_YES:{
            auto tmp = m_refTreeModel->get_iter(*str);

            std::filesystem::create_directory(std::filesystem::path(Glib::ustring((*tmp)[m_Columns->m_col_path]).raw()+"/"+entry->get_text()));
            auto c = m_refTreeModel->get_iter(*str);

            if ( entry->get_text() == ""){
                hyplog("Empty String Provided\n");
                break;
            }
            Gtk::TreeModel::Row row = *(m_refTreeModel->append(c->children()));
            row[m_Columns->m_col_name] = entry->get_text();
            row[m_Columns->m_col_path] = Glib::ustring((*tmp)[m_Columns->m_col_path]).raw()+"/"+entry->get_text();
            row[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/close0.svg"));

            sorted_model = Gtk::TreeModelSort::create(m_refTreeModel);

            sorted_model->set_sort_column(m_Columns->m_col_name, Gtk::SORT_ASCENDING);

            unset_model();set_model(sorted_model);
            expand_to_path(Gtk::TreePath(*org+":"));

        end:
            dialog->close();
            break;
        }
        case Gtk::RESPONSE_NO:
            dialog->close();
        break;

        default:

        break;
    }

}
/*
 *
 *
 *
 */
void hyp::HypTreeView::on_menu_file_addfile(){

    Gtk::Dialog *dialog = new Gtk::Dialog("Add File Name");
    Gtk::Entry *entry = new Gtk::Entry();
    dialog->add_button("Cancel",Gtk::RESPONSE_NO);
    dialog->add_button("OK",Gtk::RESPONSE_YES);

    dialog->get_vbox()->pack_start(*entry,false,false,10);
    entry->set_size_request(200,40);
    dialog->set_size_request(300,80);
    dialog->show_all();


    int result = dialog->run();

    switch (result){
        case Gtk::RESPONSE_YES:{
            auto tmp = m_refTreeModel->get_iter(*str);

            std::ofstream f(Glib::ustring((*tmp)[m_Columns->m_col_path]).raw()+"/"+entry->get_text());
            f.close();

            auto c = m_refTreeModel->get_iter(*str);


            if ( entry->get_text() == ""){
                hyplog("Empty String Provided\n");

            }else{
                Gtk::TreeModel::Row row = *(m_refTreeModel->append(c->children()));

                row[m_Columns->m_col_name] = entry->get_text();
                row[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/text2.svg"),24,24 );
                row[m_Columns->m_col_path] = Glib::ustring((*tmp)[m_Columns->m_col_path]).raw()+"/"+entry->get_text();


                sorted_model = Gtk::TreeModelSort::create(m_refTreeModel);
                sorted_model->set_sort_column(m_Columns->m_col_name, Gtk::SORT_ASCENDING);

                unset_model();set_model(sorted_model);
                expand_to_path(Gtk::TreePath(*org+":"));

            }

            dialog->close();
            break;
        }
        case Gtk::RESPONSE_NO:
            dialog->close();
            break;
        default:

            break;
    }


}
/*
 *
 *
 *
 */
bool hyp::HypTreeView::on_button_press_event(GdkEventButton* button_event){

    bool return_value = false;

    return_value = TreeView::on_button_press_event(button_event);
    auto c = m_refTreeModel->get_iter(*str);

    if( (button_event->type == GDK_BUTTON_PRESS) && (button_event->button == 3) ){
        if(Glib::file_test(Glib::ustring((*c)[m_Columns->m_col_path]).raw(),Glib::FILE_TEST_IS_DIR)){

            m_Menu_1.popup_at_pointer((GdkEvent*)button_event);
        }else{

            m_Menu_2.popup_at_pointer((GdkEvent*)button_event);
        }
    }

    return return_value;
}
/*
 *
 *
 *
 */
void hyp::HypTreeView::on_tree_click(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path){
    columns_autosize();
    check_resize();
}
