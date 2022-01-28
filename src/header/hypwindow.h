#include <gtksourceviewmm.h>
#include <gtkmm.h>
#include <vector>
#pragma once

#include <iostream>
#include <functional>
#include <set>
#include <iterator>
#include <filesystem>
#include <map>
#include <thread>
#include <list>
#include <unistd.h>
#include "../header/hyptextview.h"
#include "../header/hypimgview.h"
#include "../header/hypworker.h"
#include "../header/hyptreeview.h"



namespace hyp{

	class HypWindow : public Gtk::ApplicationWindow {
		public:
  			HypWindow();

  			virtual ~HypWindow();

  			Gtk::VBox grand_window;

			Gtk::HPaned middle_window;

			Gtk::VPaned v_window;

			Gtk::HBox toolbar;

			Gtk::VBox tree;

			Gtk::Label *file;

			Gtk::Label *status;
			
			Gtk::Button x;
			
			int count=0;
  			
  			int m_row = 0;
  			
  			int m_child = 0;
  			
  			int thr_count = 0;
			
			int icount = 0;
			
			Gtk::Notebook nb;

			std::vector<Gtk::ScrolledWindow> vec_scroll;

			std::vector<hyp::HypTextView> vec_text;

			std::vector<hyp::HypImgView> vec_imge;

  			std::map<int,std::string> *types;

			std::map<int,int> *text_track;
  			
  			std::map<int,int> *img_track;

			std::set<int> tracker;

			hyp::HypTreeView *treeView;

  			//
  			std::thread *thr;
  			// 
  			Glib::Dispatcher hyp_dispatch;

			// Gtk::ScrolledWindow for_tree;
			Gtk::ScrolledWindow *for_tree;

			Gtk::ScrolledWindow *for_shell;




  			//#####################################################

  			std::map<std::string,std::string> *folders;

  			std::set<std::string> *selected;



  			/* Class Methods which are Public:
			 *
  			 */ 
  			void set_dir(std::string fold,Gtk::TreeModel::Row &row,std::string);
			
			std::string file_type_analyze(std::string file);

			void on_thread_call();

  			//Signal handlers:
			void on_save();

			void on_save_as();

  			void on_menu_others();

			void on_new_window();
			
  			void on_menu_choices(const Glib::ustring& parameter);

  			void on_menu_choices_other(int parameter);

  			void on_menu_toggle();

			void on_tab_change(Gtk::Widget* page, guint page_number);

			void on_row_insert(const Gtk::TreeModel::Path& path,const Gtk::TreeModel::iterator& iter);

  			// tab handler
  			void insert_tab();
  			// close tab
  			void on_tab_closed(int c,std::string);
  			// 
  			void on_file_open();
  			// 
  			void on_folder_open();
			
			void on_file_select(std::string file);

  			//Child widgets:
  			Glib::RefPtr<Gtk::Builder> m_refBuilder;
  			//Two sets of choices:
  			Glib::RefPtr<Gio::SimpleAction> m_refChoice;
  			//
  			Glib::RefPtr<Gio::SimpleAction> m_refChoiceOther;
  			//
  			Glib::RefPtr<Gio::SimpleAction> m_refToggle;
	};

}
