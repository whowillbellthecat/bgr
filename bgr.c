#include "bgr.h"
#include "config.h"
#include "f.c"
#include "s.c"

V daemonize() {
	SW(fork()) {
		CSW(0, {});
		CS(-1, EF(O("cannot fork")));
		CSD(ES({}));
	}
}
V usage() { ES(O("bgr [-vd] [-c cmd] [-s seconds] path\n"));}
V version() { ES(O("bgr v 1\n")); }

S bstr(S str, ...) { /* cat arbitrary num of strings; va_list must be null terminated */
	I l; S buf;
	VA(str, l=1+strlen(str), l+=strlen(p), S);
	VA(str, buf=malloc(l); strcpy(buf, str), strcat(buf, p), S);
	buf[l-1] = '\0';
	R buf;
}
I updateimgs(IMG *f, S t, I c) { /*ureq being set implies either first run or got sigusr1 */
	I u = ureq, n = c;
	P(u, n = findimgs(f, t));
	ureq = 0;
	P(u, sigprocmask(SIG_UNBLOCK, &sigusr, NULL));
	R n;
}
I main(I argc, S* argv) {
	I n = 0, fork = 1, rdelay = 10, logopt = 0; S fp, t, cmd; C ch; cS err;
	X C *malloc_options;
	malloc_options = "X";

	while ((ch = getopt(argc, argv, "c:s:vdh")) != -1) {
		SW(ch) {
			CS('v', version());
			CSW('d', fork = 0; logopt |= LOG_PERROR);
			CSW('s', rdelay = strtonum(optarg, 1, INT_MAX, &err); P(err, EF(O("%s\n", err))));
			CSW('c', cmd = optarg);
			CSW('h', usage());
			CSD(usage());
		}
	}
	argc -= optind;
	argv += optind;
	P(--argc, usage());
	P(!cmd, cmd = defaultcmd);
	t = bstr(*argv, "/", NULL);

	P(fork, daemonize());
#ifdef __OpenBSD__
	uvs("xr", t, "/usr/", "/var/run", "/etc/", NULL);
	pledge("stdio rpath proc exec", NULL);
#endif
	openlog("bgr", logopt, LOG_USER);

	IMG *files = calloc(max_files, SZ(IMG));

	signal(sigusr, handlesigusr);
	srand(time(NULL));
	
	for (ureq = 1;;) {
		n = updateimgs(files, t, n);
		P(!n, EF(ERR("No files found in %s", t)));
		fp = bstr(t, files[(rand() / (RAND_MAX / n))].name, NULL);
		P(setbg(fp, cmd) < 0, EF({}));
		free(fp);
		sleep(rdelay);
	}
	ES({});
}