#include <gtkmm.h>
#include <gtksourceviewmm.h>



namespace hyp{
	class HypWindow : public Gtk::ApplicationWindow {
		public:
  			HypWindow();
  			virtual ~HypWindow();

  			Gtk::VBox grand_window;
			Gtk::HPaned middle_window;
			Gtk::HBox h,c;
			Gtk::VBox tab_win;

			Gtk::Button x;
			int count=0;
			Gtk::Notebook nb;

			Gtk::TextView txt_v;

			Gtk::ScrolledWindow sw;

			std::vector<Gtk::ScrolledWindow> vec_scroll;
			std::vector<Gsv::View> vec_text;

		protected:
  			//Signal handlers:
  			void on_menu_others();

  			void on_menu_choices(const Glib::ustring& parameter);
  			void on_menu_choices_other(int parameter);
  			void on_menu_toggle();


  			// tab handler
  			void insert_tab();

  			//Child widgets:
  			Gtk::Box m_Box;

  			Glib::RefPtr<Gtk::Builder> m_refBuilder;

  			//Two sets of choices:
  			Glib::RefPtr<Gio::SimpleAction> m_refChoice;
  			Glib::RefPtr<Gio::SimpleAction> m_refChoiceOther;

  			Glib::RefPtr<Gio::SimpleAction> m_refToggle;

	};

}
