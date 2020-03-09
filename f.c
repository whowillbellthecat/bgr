Z I goodex(struct dirent *dp) {
	N I sz = (SZ(ext_tb) / SZ((S)ext_tb));
	cS suffix = strrchr(dp->d_name, '.');
	U(suffix);
	DO(sz,P(!strcmp(suffix,ext_tb[i]),R 1));
	R 0;
}
Z I findimgs(IMG *fs, char *p) {
	DIR *dirh; struct dirent *dp; IMG *fp = fs; I sz = 0;
	U((dirh = opendir(p)) != NULL);
	while ((dp = readdir(dirh)) != NULL)
		P(goodex(dp), memcpy((V*)fp++, (V*)dp->d_name, SZ(IMG)); P(sz++ > max_files, break));
	P(closedir(dirh) < 0, EF(ERR("could not close dir: %m")));
	R sz;
}
