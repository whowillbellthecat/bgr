volatile sig_atomic_t ureq; /* flag to propagate sigusr */

Z pid_t setbg(S img, S cmd) {
	I ret, status = 1;
	SW(fork()) {
		CS(0, ERR("%s", img); E(execl(cmd,cmd,img,NULL)));
		CS(-1, EF(ERR("could not fork process: %m")));
	}
	wait(&status);
	ret = WEXITSTATUS(status) ? WIFEXITED(status) : WTERMSIG(status);
	P(ret != 0, WARN("%s exited with ret %d", cmd, ret));
	R ret;
}
V handlesigusr(I sig) {
	I e = errno;
	sigprocmask(SIG_BLOCK, &sigusr, NULL);
	signal(sigusr, handlesigusr);
	ureq = 1; errno = e;
}
/* input arg list must be terminated by NULL */
V uvs(S mode, ...) { VA(mode, {}, UV(p, mode), S); UV(NULL,NULL);}
