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

				zkreg.MessageRegHost mreg = new zkreg.MessageRegHost();
				mreg.regHostReq = host;

				zkreg.MessageRegHostResponse mres = reg.regHost(mreg);

				Console.Write (".");
			}
		}

		static void test_cfg()
		{
			zkconfig.zkcfg cfg = new zkconfig.zkcfg ();
			cfg.Url = "http://172.16.1.103:8899";

			zkconfig.MessageGetAllKeysResponse keys = cfg.getAllKeys (new zkconfig.MessageGetAllKeys ());

			zkconfig.MessageSetValue sv = new zkconfig.MessageSetValue ();
			sv.setValueKeyRequest = "key1";
			sv.value = "value1";
			zkconfig.Ret rc = cfg.setValue (sv);

			zkconfig.MessageGetValue gv = new zkconfig.MessageGetValue ();
			gv.getValueKeyRequest = "key1";
			rc = cfg.getValue (gv);

			keys = cfg.getAllKeys (new zkconfig.MessageGetAllKeys ());

			zkconfig.MessageDelKey dk = new zkconfig.MessageDelKey ();
			dk.delKeyRequest = "key1";
			rc = cfg.delKey (dk);

			keys = cfg.getAllKeys (new zkconfig.MessageGetAllKeys ());

		}

		public static void Main (string[] args)
		{
			test_cfg ();

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

			zkquery.zkq query = new zkquery.zkq();
            for (int i = 0; i < 100000; i++) {
                // 使用 zkquery获取 hosts 列表
				query.Url = "http://172.16.1.103:8899";
				zkquery.MessageGetAllHosts req = new zkquery.MessageGetAllHosts();
                req.getAllHostsoffline = true;
				zkquery.MessageGetAllHostsResponse res = query.getAllHosts(req);

                int num = 0;
                if (res.hosts == null) {
                    Console.WriteLine("None hosts");
                }
                else {
                    foreach (zkquery.Host host in res.hosts) {
                        Console.WriteLine(num + ": token=" + host.name);
                        num++;
                    }
                }
            }
		}
	}
}
