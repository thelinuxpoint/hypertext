#include "../header/hypertextapp.h"
#include "../header/hypwindow.h"
#include <iostream>

hyp::HypTextApp::HypTextApp(): Gtk::Application("org.gtkmm.example.main_menu"){
	Glib::set_application_name("Hyper Text");
}

Glib::RefPtr<hyp::HypTextApp> hyp::HypTextApp::create(){
  return Glib::RefPtr<hyp::HypTextApp>(new hyp::HypTextApp());
}

void hyp::HypTextApp::on_startup(){
 	//Call the base class's implementation:
  	Gtk::Application::on_startup();

  	//Create actions for menus and toolbars.
  	//We can use add_action() because Gtk::Application derives from Gio::ActionMap.

  	//File|New sub menu:
  	// add_action("newfile",sigc::mem_fun(*this, &hyp::HypTextApp::on_menu_file_new_generic));
 Gdk::RGBA black_backk;
    black_backk.set("#171815");
  	add_action("newfoo",sigc::mem_fun(*this, &hyp::HypTextApp::on_menu_file_new_generic));

  	add_action("newgoo",sigc::mem_fun(*this, &hyp::HypTextApp::on_menu_file_new_generic));

  	//File menu:
  	add_action("quit", sigc::mem_fun(*this, &hyp::HypTextApp::on_menu_file_quit));

  	//Help menu:
  	add_action("about", sigc::mem_fun(*this, &hyp::HypTextApp::on_menu_help_about));

    add_action("newwindow", sigc::mem_fun(*this, &hyp::HypTextApp::on_new_window));

    


  	m_refBuilder = Gtk::Builder::create();

  	//Layout the actions in a menubar and an application menu:
  	Glib::ustring ui_info =
    "<interface>"
    "  <!-- menubar -->"
    "  <menu id='hyper-menu'>"
    "    <submenu>"
    "      <attribute name='label' translatable='yes'>_File</attribute>"
    "      <section>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>New _File</attribute>"
    "          <attribute name='action'>win.newfile</attribute>"
    "          <attribute name='accel'>&lt;Primary&gt;n</attribute>"
    "        </item>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>Open _File</attribute>"
    "          <attribute name='action'>win.openfile</attribute>"
    "          <attribute name='accel'>&lt;Primary&gt;o</attribute>"
    "        </item>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>Open _Folder</attribute>"
    "          <attribute name='action'>win.openfolder</attribute>"
    "        </item>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>Save</attribute>"
    "          <attribute name='action'>win.save</attribute>"
    "          <attribute name='accel'>&lt;Primary&gt;s</attribute>"
    "        </item>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>Save _As</attribute>"
    "          <attribute name='action'>win.saveas</attribute>"
    "        </item>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>New _Window</attribute>"
    "          <attribute name='action'>app.newwindow</attribute>"
    "        </item>"
    "      </section>"
    "      <section>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>_Quit</attribute>"
    "          <attribute name='action'>app.quit</attribute>"
    "          <attribute name='accel'>&lt;Primary&gt;q</attribute>"
    "        </item>"
    "      </section>"
    "    </submenu>"

    "    <submenu>"
    "      <attribute name='label' translatable='yes'>_Edit</attribute>"
    "      <section>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>_Copy</attribute>"
    "          <attribute name='action'>win.copy</attribute>"
    "          <attribute name='accel'>&lt;Primary&gt;c</attribute>"
    "        </item>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>_Paste</attribute>"
    "          <attribute name='action'>win.paste</attribute>"
    "          <attribute name='accel'>&lt;Primary&gt;v</attribute>"
    "        </item>"
    "      </section>"
    "    </submenu>"

    "    <submenu>"
    "      <attribute name='label' translatable='yes'>_Preferences</attribute>"
    "      <section>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>Save _On _Focus _Lost</attribute>"
    "          <attribute name='action'>win.sometoggle</attribute>"
    "        </item>"
    "      </section>"
    "    </submenu>"

    "    <submenu>"
    "      <attribute name='label' translatable='yes'>_Help</attribute>"
    "      <section>"
    "        <item>"
    "          <attribute name='label' translatable='yes'>_About</attribute>"
    "          <attribute name='action'>win.about</attribute>"
    "        </item>"
    "      </section>"
    "    </submenu>"
    "  </menu>"
    ""
    "  <!-- application menu -->"
    "  <menu id='appmenu'>"
    
    "  </menu>"
    "</interface>";

  	try{
    	m_refBuilder->add_from_string(ui_info);
  	}
  	catch (const Glib::Error& ex){
    	std::cerr << "Building menus failed: " << ex.what();
  	}

  	//Get the menubar and the app menu, and add them to the application:
  	auto object = m_refBuilder->get_object("hyper-menu");
  	auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
  	// object = m_refBuilder->get_object("appmenu");
  	// auto appMenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
  	if (!gmenu) {
    	g_warning("GMenu or AppMenu not found");
  	}
  	else{
	    // set_app_menu(appMenu);
	    set_menubar(gmenu);
  	}
}

void hyp::HypTextApp::on_activate()
{
  //std::cout << "debug1: " << G_STRFUNC << std::endl;
  // The application has been started, so let's show a window.
  // A real application might want to reuse this window in on_open(),
  // when asked to open a file, if no changes have been made yet.
  create_window();
}

void hyp::HypTextApp::create_window(){
  	auto win = new hyp::HypWindow();

  	//Make sure that the application runs for as long this window is still open:
  	add_window(*win);

  	//Delete the window when it is hidden.
  	//That's enough for this simple example.
  	win->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this, &hyp::HypTextApp::on_window_hide), win));
  	win->show_all();
}

void hyp::HypTextApp::on_new_window(){
    auto win = new hyp::HypWindow();

    //Make sure that the application runs for as long this window is still open:
    add_window(*win);

    //Delete the window when it is hidden.
    //That's enough for this simple example.
    win->signal_hide().connect(sigc::bind<Gtk::Window*>(sigc::mem_fun(*this, &hyp::HypTextApp::on_window_hide), win));
    win->show_all();


}




void hyp::HypTextApp::on_window_hide(Gtk::Window* window){
  	delete window;
}

void hyp::HypTextApp::on_menu_file_new_generic(){
  	std::cout << "A File|New menu item was selected." << std::endl;
}

void hyp::HypTextApp::on_menu_file_quit(){
  	std::cout << "Bye ..." << std::endl;
  	quit(); // Not really necessary, when Gtk::Widget::hide() is called.

  // Gio::Application::quit() will make Gio::Application::run() return,
  // but it's a crude way of ending the program. The window is not removed
  // from the application. Neither the window's nor the application's
  // destructors will be called, because there will be remaining reference
  // counts in both of them. If we want the destructors to be called, we
  // must remove the window from the application. One way of doing this
  // is to hide the window.
  	std::vector<Gtk::Window*> windows = get_windows();
  	if (windows.size() > 0)
    	windows[0]->hide(); // In this simple case, we know there is only one window.
}

void hyp::HypTextApp::on_menu_help_about(){
  	std::cout << "App|Help|About was selected." << std::endl;
}
