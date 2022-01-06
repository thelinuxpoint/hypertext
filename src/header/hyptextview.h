#include <gtksourceviewmm.h>

#include <gtkmm.h>
#include <vector>
#include <iostream>
#include <functional>
#include <set>
#include <iterator>

namespace hyp{

	class HypTextView : public Gsv::View{
		public:
			std::string file_name;
			std::string path;
			bool is_saved;
			bool is_defined;
			int id;
			
			HypTextView(std::string name,std::string path,int id);
	};
}
