#include "../header/hyptextview.h"

hyp::HypTextView::HypTextView(std::string name,std::string path,int id){
	Gsv::View();
	this->file_name = name;
	this->path = path;
	this->id = id;
}
