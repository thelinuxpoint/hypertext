
#include "../header/hypwindow.h"
/*[#] Constructor:
 *		
 *
 */ 
hyp::HypTreeView::HypTreeView(hyp::HypWindow *parent){
	
	
	//

	Gdk::RGBA black_backk;black_backk.set("#151515");
	//
	m_Columns = new ModelColumns();
	// 
    m_refTreeModel = Gtk::TreeStore::create(*m_Columns);
    // initialize the treeview constructor
    Gtk::TreeView();
    // set the tree model
    set_model(m_refTreeModel);
	// set color for background
	override_background_color(black_backk);
	//
	set_vexpand();
	//
	set_enable_tree_lines( true);
	//
	m_tree_selector = get_selection();
    m_tree_selector->set_select_function(sigc::bind(sigc::mem_fun(*this,&hyp::HypTreeView::on_row_select),parent));

	hpy_column = Gtk::manage(new Gtk::TreeViewColumn());
    //
    cell_pix = Gtk::manage(new Gtk::CellRendererPixbuf());
    //
    cell_txt = Gtk::manage(new Gtk::CellRendererText());
    // 
	folders = new std::map<std::string,std::string>();
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

	signal_row_expanded().connect( sigc::mem_fun(*this,&hyp::HypTreeView::on_tree_click));

    int x = append_column(*hpy_column);
    get_column(x-1)->add_attribute(*cell_pix,"stock-id",m_Columns->m_col_name);

}
/*[#] Opens File when column of TreeView is Clicked:
 *		
 *
 *
 */ 
bool hyp::HypTreeView::on_row_select(const Glib::RefPtr<Gtk::TreeModel>& b,const Gtk::TreeModel::Path& c,bool a,hyp::HypWindow *parent){
    // for Debuging only:
    // std::cout<<"Selected ... "<<std::flush<<c.to_string()<<std::endl;
    show_all();
    if(std::filesystem::is_regular_file((*folders)[c.to_string()]) and (selected->count((*folders)[c.to_string()]) == 0) ){
        
        parent->on_file_select((*folders)[c.to_string()]);

        parent->file->set_label(file_type_analyze(std::filesystem::path((*folders)[c.to_string()]).filename().string()));
        
        selected->insert((*folders)[c.to_string()]);
        return false;
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
    // parent->middle_window.set_position(250);
    caller->notify();
    show_all();
}
/*[#] Add Folder Main:
 *		
 *
 *
 */
void hyp::HypTreeView::set_dir(std::string fold,Gtk::TreeModel::Row &row,std::string x){
    row[m_Columns->m_col_name] = std::filesystem::path(fold).filename().string();
    x=x+":";
    int m_child=0;
    
    for(auto const& dir_entry: std::filesystem::directory_iterator{std::filesystem::path(fold)}){

        if (Glib::file_test(dir_entry.path().string(),Glib::FILE_TEST_IS_DIR)){

            // std::cout<<"Directory: "<<dir_entry.path().string()<<std::endl;
            
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
            }else if(dir_entry.path().filename().extension().string() == ".rs"){
                childrow[m_Columns->m_col_pix] = Gdk::Pixbuf::create_from_file( (std::string(get_current_dir_name())+"/src/resource/rust/rust.svg"),22,22 );
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
            
            childrow[m_Columns->m_col_name] = dir_entry.path().filename().string();

            (*folders)[(x+std::to_string(m_child))] = dir_entry.path().string();
            
            // for Debuging only:
            // std::cout<<(x+std::to_string(m_child))<<std::endl;
            m_child++;        
        }
    }
   	show_all();
}

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


void hyp::HypTreeView::on_tree_click(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path){
    columns_autosize();
    check_resize();
}
