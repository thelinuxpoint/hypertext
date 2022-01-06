#include <gtkmm.h>
#include <gtksourceviewmm.h>
#include <set>
#include <list>
#include "../header/hyptextview.h"

namespace hyp{
	class HypWindow : public Gtk::ApplicationWindow {
		public:
  			HypWindow();
  			virtual ~HypWindow();

  			Gtk::VBox grand_window;
			Gtk::HPaned middle_window;
			Gtk::HBox h,c;
			Gtk::VBox tree;

			Gtk::Button x;
			int count=0;
			Gtk::Notebook nb;
			Gtk::ScrolledWindow *sw_nb;
			Gtk::TextView txt_v;
			Gtk::ScrolledWindow sw;
			std::vector<Gtk::ScrolledWindow> vec_scroll;
			std::vector<hyp::HypTextView> vec_text;
			// Gtk::ScrolledWindow for_tree;
			std::set<int> tracker;
			  //Tree model columns:
  			class ModelColumns : public Gtk::TreeModel::ColumnRecord{
  				public:
    				ModelColumns()
    				{  
    					add(m_col_pict);
    					add(m_col_name);
    				}

    				Gtk::TreeModelColumn<Glib::RefPtr<Gdk::Pixbuf> > m_col_pict;
    				Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  			};

  			ModelColumns m_Columns;

			Gtk::TreeView *m_TreeView;
  			Glib::RefPtr<Gtk::TreeStore> m_refTreeModel;


  			void on_tab_closed(int c);

  			void on_file_open();

  			void on_folder_open();

  			void set_dir(std::string fold,Gtk::TreeModel::Row &row);

		protected:
  			//Signal handlers:
  			void on_menu_others();

  			void on_menu_choices(const Glib::ustring& parameter);
  			void on_menu_choices_other(int parameter);
  			void on_menu_toggle();


  			// tab handler
  			void insert_tab();
  			// close tab

  			//Child widgets:
  			Gtk::Box m_Box;

  			Glib::RefPtr<Gtk::Builder> m_refBuilder;

  			//Two sets of choices:
  			Glib::RefPtr<Gio::SimpleAction> m_refChoice;
  			Glib::RefPtr<Gio::SimpleAction> m_refChoiceOther;

  			Glib::RefPtr<Gio::SimpleAction> m_refToggle;

	};

}
