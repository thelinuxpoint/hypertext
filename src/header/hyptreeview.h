#include <gtksourceviewmm.h>
#include <gtkmm.h>
#include <vector>
#include <iostream>
#include <functional>
#include <set>
#include <iterator>
#include <filesystem>
#include <map>
#include <thread>
#include <list>
#include <unistd.h>




namespace hyp{
	class HypWindow;
	
	class HypTreeView: public Gtk::TreeView {
		
		public:
		HypTreeView(hyp::HypWindow *parent);


//Tree model columns:
  			class ModelColumns : public Gtk::TreeModel::ColumnRecord{
  				public:
    				ModelColumns(){ 	
    					add(m_col_name); 
    					add(m_col_pix);
    				}
    				Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    				Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf>> m_col_pix;
  			};
//			
			ModelColumns *m_Columns;
//
  			Gtk::TreeViewColumn *hpy_column;
			Gtk::CellRendererPixbuf *cell_pix;
			Gtk::CellRendererText *cell_txt;

  			Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;
  			Glib::RefPtr<Gtk::TreeSelection> m_tree_selector;

	Glib::RefPtr<Gtk::TreeModelSort> sorted_model;

  			std::map<std::string,std::string> *folders;

  			std::set<std::string> *selected;


  			int m_row = 0;
			std::mutex mtx;


  			bool on_row_select(const Glib::RefPtr<Gtk::TreeModel>& b,const Gtk::TreeModel::Path& c,bool a,hyp::HypWindow *parent);
//
  			void add_folder(std::string path,hyp::HypWindow *caller);

  			void set_dir(std::string fold,Gtk::TreeModel::Row &row,std::string x);
			
			void on_tree_click(const Gtk::TreeModel::iterator& iter, const Gtk::TreeModel::Path& path);

			std::string file_type_analyze(std::string file);
	};
}
