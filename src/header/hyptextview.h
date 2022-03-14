#include <gtksourceviewmm.h>
#include <filesystem>
#include <gtkmm.h>
#include <vector>
#include <iostream>
#include <functional>
#include <set>
#include <iterator>

namespace hyp{

	class HypTextView : public Gsv::View {
		public:
			std::string file_name;
			std::string path;
			std::string file_type;

			Glib::RefPtr<Gio::FileMonitor> mm_file;

			Gtk::Label *l;
			bool is_saved;
			bool is_defined;
			int id;

			Glib::RefPtr<Gsv::Buffer> buffer;

			HypTextView(std::string name,std::string path,Gtk::Label *label,int id);

  			bool on_key_press_event(GdkEventKey* event) override;

			void on_buffer_changed(Gtk::Label *l);

			void on_insert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring& text, int bytes);

			void file_type_analyze(std::string);

			void on_buffer_changed(std::string x);

			void refresh();

			void defined();
	};
}
