using System;
using System.Threading;

namespace sysmgrt.net
{
	class MainClass
	{
		static void proc_test()
		{
			zkreg reg = new zkreg ();
			reg.Url = "http://localhost:8899";

			for (int i = 0; i < 1000; i++) {

				Host host = new Host ();
				host.name = Guid.NewGuid ().ToString ("D");
				host.catalog = Catalog.Host;
				host.ips = new string[1];
				host.ips [0] = "192.168.1.10";
				host.showname = "汉字";

				MessageRegHost mreg = new MessageRegHost ();
				mreg.regHostReq = host;

				MessageRegHostResponse mres = reg.regHost (mreg);

				Console.WriteLine ("token=" + mres.token);
			}
		}

		public static void Main (string[] args)
		{
			int count = 10;
			Thread []th = new Thread[count];

			for (int i = 0; i < count; i++) {
				th [i] = new Thread (new ThreadStart (proc_test));
				th [i].Start ();
			}

			for (int i = 0; i < count; i++) {
				th [i].Join (-1);
			}
		}
	}
}
