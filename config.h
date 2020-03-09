Z N sigset_t sigusr = SIGUSR1; /* signals that should indicate to update known files list */
Z N I max_files = 200; /* upper bound on number of files we read */
Z N S defaultcmd = "/usr/local/bin/bgs"; /* command to set bg */

Z cS ext_tb[] = { /* use files matching these file extensions */
	".png",
	".jpg",
	".jpeg"
};
