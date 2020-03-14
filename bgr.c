#include "bgr.h"
#include "config.h"
#include "f.c"
#include "s.c"

V usage() { ES(O("bgr [-vd] [-c cmd] [-s seconds] path\n"));}
S bstr(S str, ...) { /* cat arbitrary num of strings; va_list must be null terminated */
	I l; S buf;
	VA(str, l=1+strlen(str), l+=strlen(p), S);
	VA(str, buf=malloc(l); U(buf); strcpy(buf, str), strcat(buf, p), S);
	buf[l-1] = '\0';
	R buf;
}
I main(I argc, S* argv) {
	I n = 0, fork = 1, rdelay = 10, logopt = 0; S fp, t, cmd = NULL; C ch; cS err;
	while ((ch = getopt(argc, argv, "hc:s:d")) != -1) {
		SW(ch) {
			CSW('d', fork = 0; logopt |= LOG_PERROR);
			CSW('s', rdelay = strtonum(optarg, 1, INT_MAX, &err); P(err, EF(O("%s\n", err))));
			CSW('c', cmd = optarg);
			CSD(usage());
		}
	}
	argc -= optind;
	argv += optind;
	P(--argc, usage());
	P(!cmd, cmd = defaultcmd);
	EQF(t, bstr(*argv, "/", NULL), "malloc");

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
		EQF(n,updateimgs(files, t, n), "no files match");
		EQF(fp, bstr(t, files[(rand() / (RAND_MAX / n))].name, NULL), "malloc");
		P(setbg(fp, cmd) < 0, EF({}));
		(void)free(fp);
		(void)sleep(rdelay);
	}
	ES({});
}
