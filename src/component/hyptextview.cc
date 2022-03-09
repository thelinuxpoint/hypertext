#include "../header/hyptextview.h"

hyp::HypTextView::HypTextView(std::string name,std::string path,Gtk::Label *label,int id){
	Gsv::init();
	Gsv::View();

	this->file_name = name;
	this->path = path;
	this->id = id;
	Gdk::RGBA grey;
    grey.set("#272822");

    Glib::RefPtr<Gsv::StyleSchemeManager> style = Gsv::StyleSchemeManager::create();
	style->append_search_path("/usr/share/gtksourceview-3.0/styles/Monokai.xml");
	auto vec_x = style->get_scheme_ids();
	set_auto_indent();
	// for (std::vector<std::string>::iterator i = vec_x.begin(); i != vec_x.end(); ++i)
	// {
	// 	std::cout<<*i<<std::endl;
	// }
	l = label;
    // override_background_color(grey);
	set_show_line_marks();
	set_indent_on_tab();
	set_indent_width(4);



	if(this->path!=""){
		Glib::RefPtr<Gsv::Language> lang;
		Glib::RefPtr<Gsv::LanguageManager> x = Gsv::LanguageManager::create();

		bool result_uncertain = FALSE;
    	Glib::ustring content_type;
    	content_type = Gio::content_type_guess(this->file_name, 0, 0, result_uncertain);
    	if (result_uncertain){
      		content_type.clear();
    	}

   		lang = x->guess_language(this->file_name, content_type);
   		Glib::RefPtr<Gsv::Buffer> buffer = Gsv::Buffer::create(lang);

    	set_buffer(buffer);
   		buffer->set_style_scheme(style->get_scheme(vec_x[5]));
    	buffer->set_text(Glib::file_get_contents(this->path));
    	buffer->set_highlight_syntax();
	}else{
		Glib::RefPtr<Gsv::Language> lang;
   		Glib::RefPtr<Gsv::Buffer> buffer = Gsv::Buffer::create(lang);
		set_buffer(buffer);
   		buffer->set_style_scheme(style->get_scheme(vec_x[5]));
	}

	// std::cout<<get_name()<<std::endl;

	file_type_analyze(name);
	set_accepts_tab(true);
	set_indent(4);
	set_tab_width(4);

	// get_buffer()->signal_changed().connect(sigc::bind(sigc::mem_fun(*this,&hyp::HypTextView::on_buffer_changed),label));
	// get_buffer()->signal_insert().connect(sigc::mem_fun(*this,&hyp::HypTextView::on_insert));
	// signal_key_press_event().connect( sigc::ptr_fun(&hyp::on_buffer_changed));
}
// ######################################################################

bool hyp::HypTextView::on_key_press_event(GdkEventKey* key_event)
{

	this->View::on_key_press_event(key_event);

	if (this->path != ""){
		std::string x = Glib::file_get_contents(this->path);

		if(x != get_buffer()->get_text()){
			l->set_label(this->file_name+" ●");
		}else{
			l->set_label(this->file_name);
		}
	}
	return true;
}


void hyp::HypTextView::defined(){

	Glib::RefPtr<Gsv::StyleSchemeManager> style = Gsv::StyleSchemeManager::create();
	style->append_search_path("/usr/share/gtksourceview-3.0/styles/Monokai.xml");
	auto vec_x = style->get_scheme_ids();

	if(this->path!=""){
		Glib::RefPtr<Gsv::Language> lang;
		Glib::RefPtr<Gsv::LanguageManager> x = Gsv::LanguageManager::create();

		bool result_uncertain = FALSE;
    	Glib::ustring content_type;
    	content_type = Gio::content_type_guess(this->file_name, 0, 0, result_uncertain);
    	if (result_uncertain){
      		content_type.clear();
    	}

   		lang = x->guess_language(this->file_name, content_type);
   		Glib::RefPtr<Gsv::Buffer> buffer = Gsv::Buffer::create(lang);

    	set_buffer(buffer);

   		buffer->set_style_scheme(style->get_scheme(vec_x[5]));
    	buffer->set_text(Glib::file_get_contents(this->path));

	}
}





void hyp::HypTextView::file_type_analyze(std::string file){
	if(std::filesystem::path(file).extension().string()==".cc" or std::filesystem::path(file).extension().string()==".cpp"){
		this->file_type="C++";
	}else if(std::filesystem::path(file).extension().string()==".c"){
		this->file_type="C";
	}else if(std::filesystem::path(file).extension().string()==".h"){
		this->file_type="C/C++ header";
	}else if(std::filesystem::path(file).extension().string()==".py"){
		this->file_type="Python";
	}else if(std::filesystem::path(file).extension().string()==".md"){
		this->file_type="Markdown";
	}else if(std::filesystem::path(file).filename().string()=="Makefile" or std::filesystem::path(file).filename().string()=="MAKEFILE"){
		this->file_type="Makefile";
	}else if(std::filesystem::path(file).extension().string()==".rs"){
		this->file_type="Rust";
	}else if(std::filesystem::path(file).extension().string()==".js"){
		this->file_type="Javascript";
	}else if(std::filesystem::path(file).extension().string()==".svg"){
		this->file_type="XML";
	}else if(std::filesystem::path(file).extension().string()==".html"){
		this->file_type="HTML";
	}else if(std::filesystem::path(file).extension().string()==".java"){
		this->file_type="Java";
	}else if(std::filesystem::path(file).extension().string()==".json"){
		this->file_type="JSON";
	}else{
		this->file_type="Plain Text";
	}
}
