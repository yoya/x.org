#ifdef hpux
#define ccflags "-Wc,-Nd4000,-Ns3000 -DSYSV"
#else
#define ccflags "-O"
#endif

main()
{
	write(1, ccflags, sizeof(ccflags) - 1);
	exit(0);
}
