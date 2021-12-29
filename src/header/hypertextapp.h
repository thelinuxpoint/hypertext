#include <gtkmm.h>

namespace hyp{

	class HypTextApp: public Gtk::Application {
		
		private:
			void create_window();
  			void on_window_hide(Gtk::Window* window);
  			void on_menu_file_new_generic();
  			void on_menu_file_quit();
  			void on_menu_help_about();

  			Glib::RefPtr<Gtk::Builder> m_refBuilder;

		protected:
  		//Overrides of default signal handlers:
			HypTextApp();
  			
  			void on_startup() override;
  			void on_activate() override;

		public:
        /*##############################################*/ 
 			static Glib::RefPtr<HypTextApp> create();
			// constructor
			
		};
}
