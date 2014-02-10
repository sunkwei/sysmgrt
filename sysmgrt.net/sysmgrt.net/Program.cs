using System;
using System.Threading;

namespace sysmgrt.net
{
	class MainClass
	{
		static void proc_test()
		{
			zkreg.zkreg reg = new zkreg.zkreg ();
			reg.Url = "http://localhost:8899";

			for (int i = 0; i < 1000; i++) {

                zkreg.Host host = new zkreg.Host();
				host.name = Guid.NewGuid ().ToString ("D");
                host.catalog = zkreg.Catalog.Host;
				host.ips = new string[1];
				host.ips [0] = "192.168.1.10";
				host.showname = "汉字";

                zkreg.regHost mreg = new zkreg.regHost();
				mreg.regHostReq = host;

                zkreg.regHostResponse mres = reg.regHost(mreg);

				Console.Write (".");
			}
		}

		public static void Main (string[] args)
		{
			#if false
			int count = 10;
			Thread []th = new Thread[count];

			for (int i = 0; i < count; i++) {
				th [i] = new Thread (new ThreadStart (proc_test));
				th [i].Start ();
			}

			for (int i = 0; i < count; i++) {
				th [i].Join (-1);
			}
			#endif

            for (int i = 0; i < 100000; i++) {
                zkquery.zkq query = new zkquery.zkq();
                // 使用 zkquery获取 hosts 列表
                query.Url = "http://localhost:8899";
                zkquery.getAllHosts req = new zkquery.getAllHosts();
                req.getAllHostsoffline = true;
                zkquery.getAllHostsResponse res = query.getAllHosts(req);

                int num = 0;
                if (res.hosts == null) {
                    Console.WriteLine("None hosts");
                }
                else {
                    foreach (zkquery.Host host in res.hosts) {
//                        Console.WriteLine(num + ": token=" + host.name);
                        num++;
                    }
                }
            }
		}
	}
}
