#include "../header/hyptextview.h"

hyp::HypTextView::HypTextView(std::string name,std::string path,Gtk::Label *label,int id){
	Gsv::View();
	this->file_name = name;
	this->path = path;
	this->id = id;
	set_accepts_tab(true);
	set_indent(4);
	this->buffer->signal_changed().connect(sigc::bind(sigc::mem_fun(*this,&hyp::HypTextView::on_buffer_changed),label));
}

void hyp::HypTextView::on_buffer_changed(Gtk::Label *l){
	std::cout<<"changed"<<std::flush<<std::endl;
	l->set_label(this->file_name+" * ");

}
