volatile sig_atomic_t ureq; /* flag to propagate sigusr */

V daemonize() {
	SW(fork()) {
		CSW(0, {});
		CS(-1, EF(O("cannot fork")));
		CSD(ES({}));
	}
}
Z pid_t setbg(S img, S cmd) {
	I ret, status = 1;
	SW(fork()) {
		CS(0, ERR("%s", img); E(execlp(cmd,cmd,img,NULL)));
		CS(-1, EF(ERR("could not fork process: %m")));
	}
	wait(&status);
	EQC(ret,WEXITSTATUS(status)?WIFEXITED(status):WTERMSIG(status),ret!=0,WARN("%s : ret %d",cmd,ret));
	R ret;
}
V handlesigusr(I sig) {
	I e = errno;
	sigprocmask(SIG_BLOCK, &sigusr, NULL);
	signal(sigusr, handlesigusr);
	ureq = 1; errno = e;
}
I updateimgs(IMG *f, S t, I c) {
	I u = ureq, n = c;
	P(u, n = findimgs(f, t));
	ureq = 0;
	P(u, sigprocmask(SIG_UNBLOCK, &sigusr, NULL));
	R n;
}
/* input arg list must be terminated by NULL */
V uvs(S mode, ...) { VA(mode, {}, unveil(p, mode), S); unveil(NULL,NULL);}
