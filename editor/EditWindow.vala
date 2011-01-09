using Gtk;

namespace Vdeck {
  class EditWindow : Window {
    public EditWindow.with_builder() {
      try {
        var b = new Builder ();
        b.add_from_file (Path.build_filename(RESOURCE_DIR,"editor.xml"));
        b.connect_signals (null);

	win = b.get_object ("editor_win") as Window;
      } catch (Error e) {
        stderr.printf ("Could not load UI: %s\n", e.message);
        win = null;
      }
    }

    public Window? win;

    private Builder builder;

    public void open_path(string filename) {
      return;
    }
  }
}
