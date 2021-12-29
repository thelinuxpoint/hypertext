#include "../header/window.h"

#include <gtkmm.h>
#include <vector>
#include <iostream>

hyp::HypWindow::HypWindow(): Gtk::ApplicationWindow(), m_Box(Gtk::ORIENTATION_VERTICAL){
    set_title("Hyper Text");
    set_default_size(600, 500);

    // ExampleApplication displays the menubar. Other stuff, such as a toolbar,
    // is put into the box.
    add(grand_window);

  // Create actions for menus and toolbars.
  // We can use add_action() because Gtk::ApplicationWindow derives from Gio::ActionMap.
  // This Action Map uses a "win." prefix for the actions.
  // Therefore, for instance, "win.copy", is used in ExampleApplication::on_startup()
  // to layout the menu.

  //Edit menu:
    add_action("copy", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));
    add_action("paste", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));
    add_action("something", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));

    //Choices menus, to demonstrate Radio items,
     //using our convenience methods for string and int radio values:
    m_refChoice = add_action_radio_string("choice",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_choices), "a");

    m_refChoiceOther = add_action_radio_integer("choiceother",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_choices_other), 1);

    m_refToggle = add_action_bool("sometoggle",sigc::mem_fun(*this, &hyp::HypWindow::on_menu_toggle), false);
    //Help menu:
    add_action("about", sigc::mem_fun(*this, &hyp::HypWindow::on_menu_others));

    //Create the toolbar and add it to a container widget:

    m_refBuilder = Gtk::Builder::create();

    Glib::ustring ui_info =
    "<!-- Generated with glade 3.18.3 -->"
    "<interface>"
    "  <requires lib='gtk+' version='3.4'/>"
    "  <object class='GtkToolbar' id='toolbar'>"
    "    <property name='visible'>True</property>"
    "    <property name='can_focus'>False</property>"
    "    <child>"
    "      <object class='GtkToolButton' id='toolbutton_new'>"
    "        <property name='visible'>True</property>"
    "        <property name='can_focus'>False</property>"
    "        <property name='tooltip_text' translatable='yes'>New Standard</property>"
    "        <property name='action_name'>app.newstandard</property>"
    "        <property name='icon_name'>document-new</property>"
    "      </object>"
    "      <packing>"
    "        <property name='expand'>False</property>"
    "        <property name='homogeneous'>True</property>"
    "      </packing>"
    "    </child>"
    "    <child>"
    "      <object class='GtkToolButton' id='toolbutton_quit'>"
    "        <property name='visible'>True</property>"
    "        <property name='can_focus'>False</property>"
    "        <property name='tooltip_text' translatable='yes'>Quit</property>"
    "        <property name='action_name'>app.quit</property>"
    "        <property name='icon_name'>application-exit</property>"
    "      </object>"
    "      <packing>"
    "        <property name='expand'>False</property>"
    "        <property name='homogeneous'>True</property>"
    "      </packing>"
    "    </child>"
    "  </object>"
    "</interface>";

    
    try{
        m_refBuilder->add_from_string(ui_info);
    }
    catch (const Glib::Error& ex){
        std::cerr << "Building toolbar failed: " <<  ex.what();
    }
    Gtk::Toolbar* toolbar = nullptr;
    m_refBuilder->get_widget("toolbar", toolbar);
    if (!toolbar)
        g_warning("GtkToolbar not found");
    else
        grand_window.pack_end(*toolbar, Gtk::PACK_SHRINK);
}

hyp::HypWindow::~HypWindow(){}

void hyp::HypWindow::on_menu_others(){
  std::cout << "A menu item was selected." << std::endl;
}

void hyp::HypWindow::on_menu_choices(const Glib::ustring& parameter)
{
  //The radio action's state does not change automatically:
  m_refChoice->change_state(parameter);

  Glib::ustring message;
  if (parameter == "a")
    message = "Choice a was selected.";
  else
    message = "Choice b was selected.";

  std::cout << message << std::endl;
}

void hyp::HypWindow::on_menu_choices_other(int parameter)
{
  //The radio action's state does not change automatically:
  m_refChoiceOther->change_state(parameter);

  Glib::ustring message;
  if (parameter == 1)
    message = "Choice 1 was selected.";
  else
    message = "Choice 2 was selected.";

  std::cout << message << std::endl;
}

void hyp::HypWindow::on_menu_toggle()
{
  bool active = false;
  m_refToggle->get_state(active);

  //The toggle action's state does not change automatically:
  active = !active;
  m_refToggle->change_state(active);

  Glib::ustring message;
  if (active)
    message = "Toggle is active.";
  else
    message = "Toggle is not active.";

  std::cout << message << std::endl;
}
