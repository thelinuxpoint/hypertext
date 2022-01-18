#include <gtkmm.h>
#include <iostream>
#include <string>
#include <filesystem>


namespace hyp{
	class HypImgView : public Gtk::Image{

		public:
			std::string name;
			std::string path;
			std::string file_type;

			int id;

			HypImgView(std::string name, std::string path, int id);
			void file_type_analyze(std::string file);
	};
}
