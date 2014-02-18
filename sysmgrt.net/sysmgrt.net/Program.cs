using System;
using System.Threading;
using System.Collections.Generic;

namespace sysmgrt.net
{
	class MainClass
	{
		static void proc_test()
		{
			zkreg.zkreg reg = new zkreg.zkreg ();
			reg.Url = "http://localhost/soap";

			for (int i = 0; i < 1000; i++) {

                zkreg.Host host = new zkreg.Host();
				host.name = Guid.NewGuid ().ToString ("D");
                host.catalog = zkreg.Catalog.Host;
				host.ips = new string[2];
				host.ips [0] = "192.168.1.10";
				host.ips [1] = "172.16.1.10";
				host.showname = "汉字";

				zkreg.MessageRegHost mreg = new zkreg.MessageRegHost();
				mreg.regHostReq = host;

				zkreg.MessageRegHostResponse mres = reg.regHost(mreg);

				Console.Write (".");
			}
		}

		static List<string> tokens_ = new List<string>();
		static void proc_heartBeat()
		{
			for (; ;) {
				System.Threading.Thread.Sleep (3 * 60 * 1000);	// 3分钟
				lock (tokens_) {
					foreach (string token in tokens_) {
						zkreg.MessageHeartBeat b = new zkreg.MessageHeartBeat ();
						b.heartBeatTokenReq = token;

						zkreg.zkreg reg = new zkreg.zkreg ();
						reg.Url = "http://localhost/soap";
						zkreg.MessageHeartBeatResponse rs = reg.heartBeat (b);
					}
				}
			}
		}

		static void regService(string name, string hostname, string type, string urls, string version)
		{
			zkreg.zkreg reg = new zkreg.zkreg ();
			reg.Url = "http://localhost/soap";

			zkreg.MessageRegService s = new zkreg.MessageRegService ();
			s.regServiceReq = new zkreg.Service ();
			s.regServiceReq.name = name;
			s.regServiceReq.catalog = zkreg.Catalog.Service;
			s.regServiceReq.hostname = hostname;
			s.regServiceReq.type = type;
			s.regServiceReq.urls = new string[1];
			s.regServiceReq.urls [0] = urls;
			s.regServiceReq.version = version;

			zkreg.MessageRegServiceResponse res = reg.regService (s);
			Console.WriteLine ("regService: token=" + res.token);

			lock (tokens_) {
				tokens_.Add (res.token);
			}
		}

		static void test_regServices()
		{
			// 注册一大堆服务
			string name = "service-";

			for (int i = 0; i < 100; i++) {
				regService (name + i, "test_host", "test", "test://1283", "0.1");
			}
		}

		static void test_cfg()
		{
			zkconfig.zkcfg cfg = new zkconfig.zkcfg ();
			cfg.Url = "http://localhost/soap";

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

		static void version()
		{
			zkreg.zkreg reg = new zkreg.zkreg ();
			reg.Url = "http://localhost/soap";

			zkreg.MessageVersionResponse res = reg.version (new zkreg.MessageVersion ());

			Console.WriteLine (res.info);
		}

		public static void Main (string[] args)
		{
			version ();

			//test_cfg ();

			#if true
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

			test_regServices ();

			Thread th_hb = new Thread (new ThreadStart (proc_heartBeat));
			th_hb.Start ();

			zkquery.zkq query = new zkquery.zkq();
			for (int i = 0; i < 100000; i++) {
				Thread.Sleep (50);
                // 使用 zkquery获取 hosts 列表
				query.Url = "http://localhost/soap";
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

				Console.WriteLine ("/////" + i + "///////");
            }
		}
	}
}
