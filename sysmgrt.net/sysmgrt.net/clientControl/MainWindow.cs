using System;
using Gtk;
using System.Threading;

public partial class MainWindow: Gtk.Window
{
	public MainWindow () : base (Gtk.WindowType.Toplevel)
	{
		Build ();
		this.Title = "看看汉字接口";

		TreeViewColumn col1 = new TreeViewColumn ();
		col1.Title = "序号";

		TreeViewColumn col2 = new TreeViewColumn ();
		col2.Title = "data";

		treeview1.AppendColumn (col1);
		treeview1.AppendColumn (col2);

		ListStore store = new ListStore (typeof(string), typeof(string));
		treeview1.Model = store;

		ShowAll ();
	}

	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Application.Quit ();
		a.RetVal = true;
	}
}
