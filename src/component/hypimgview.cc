#include "../header/hypimgview.h"

hyp::HypImgView::HypImgView(std::string _name,std::string path,int id):Gtk::Image(path){
	
	this->path = path;
	this->name = _name;
	this->id = id;
    file_type_analyze(_name);

}
void hyp::HypImgView::file_type_analyze(std::string file){
	if(std::filesystem::path(file).extension().string()==".png"){
		this->file_type="PNG";
	}else if(std::filesystem::path(file).extension().string()==".jpg"){
		this->file_type="JPG";
	}else if(std::filesystem::path(file).extension().string()==".jpeg"){
		this->file_type="JPEG";
	}else{
		this->file_type="None";
	}
}
