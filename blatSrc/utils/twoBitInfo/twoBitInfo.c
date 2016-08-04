/* twoBitInfo - get information about sequences in a .2bit file. */
#include "common.h"
#include "options.h"
#include "twoBit.h"


void usage()
/* Explain usage and exit. */
{
errAbort(
  "twoBitInfo - get information about sequences in a .2bit file\n"
  "usage:\n"
  "   twoBitInfo input.2bit output.tab\n"
  "options:\n"
  "   -nBed   instead of seq sizes, output BED records that define \n"
  "           areas with N's in sequence\n"
  "   -noNs   outputs the length of each sequence, but does not count Ns \n"
  "Output file has the columns::\n"
  "   seqName size\n"
  "\n"
  "The 2bit file may be specified in the form path:seq or path:seq1,seq2,seqN...\n"
  "so that information is returned only on the requested sequence(s).\n"
  "If the form path:seq:start-end is used, start-end is ignored.\n"
  );
}

static struct optionSpec options[] = {
   {"nBed", OPTION_BOOLEAN},
   {"noNs", OPTION_BOOLEAN},
   {NULL, 0},
};


void twoBitInfo(char *inName, char *outName)
/* twoBitInfo - get information about sequences in a .2bit file. */
{
struct twoBitFile *tbf;
FILE *outFile;
char *seqName = NULL;

twoBitParseRange(inName, &inName, &seqName, NULL, NULL);
tbf = twoBitOpen(inName);
outFile = mustOpen(outName, "w");

if (seqName != NULL)
    {
    char *seqArray[1023];
    int i;
    int seqCount = chopString(seqName, ",", seqArray, ArraySize(seqArray));
    for (i = 0 ; i < seqCount ; i++)
	{
	if (optionExists("nBed"))
	    twoBitOutNBeds(tbf, seqArray[i], outFile);
	else if(optionExists("noNs"))
	    fprintf(outFile, "%s\t%d\n", seqArray[i], twoBitSeqSizeNoNs(tbf, seqArray[i]));
	else
	    fprintf(outFile, "%s\t%d\n", seqArray[i], twoBitSeqSize(tbf, seqArray[i]));
	}
	
    }
else
    {
    struct twoBitIndex *index;
    for (index = tbf->indexList; index != NULL; index = index->next)
	{
	if (optionExists("nBed"))
	    twoBitOutNBeds(tbf, index->name, outFile);
	else if(optionExists("noNs"))
	    fprintf(outFile, "%s\t%d\n", index->name, twoBitSeqSizeNoNs(tbf, index->name));
	else
	    fprintf(outFile, "%s\t%d\n", index->name, twoBitSeqSize(tbf, index->name));
	}
    }
twoBitClose(&tbf);
carefulClose(&outFile); 
}

int main(int argc, char *argv[])
/* Process command line. */
{
optionInit(&argc, argv, options);
if (argc != 3)
    usage();
twoBitInfo(argv[1], argv[2]);
return 0;
}
