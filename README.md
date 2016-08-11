# BLATq

BLATq is the same as BLAT v. 35 (Kent, 2002; Kent, 2012), except that it will accept fastq files, although it does not utilize the base quality score information in the fastq files.

### Contents

README.md : this README

blatq_mods.md : describes what portions on the BLAT v. 35 code were changed to create blatq Version 1.0.0.

blatSrc : directory with all BLATq source code  

### How to compile

$ git clone https://github.com/calacademy-research/blatq.git  
\# Now either follow the instructions in the README found in the blatq/blastSrc directory or follow my interpreted version below.  
\# Define the MACHTYPE environment variable  
$ export MACHTYPE=\`uname -m\`  
\# Create the directory ~/bin/$MACHTYPE  
$ mkdir -p ~/bin/$MACHTYPE  
\# Add this directory to your PATH  
$ export PATH="$PATH:~/bin/$MACHTYPE"  
$ cd blatq/blatSrc  
$ make  
\# Call the executable with "blat" or make a symbolic link of blatq to blat  
$ cd ~/bin/$MACHTYPE  
$ ln -s blat blatq  

### Usage

$ blatq

   blat - Standalone BLAT v. 35 fast sequence search command line tool
   usage:
      blat database query [-ooc=11.ooc] output.psl
   where:
      database and query are each either a .fa , .nib or .2bit file,
      or a list these files one file name per line.
      NOTE: fastq files (4 lines per record) allowed in this version.  
      
      -ooc=11.ooc tells the program to load over-occurring 11-mers from
                  and external file.  This will increase the speed
                  by a factor of 40 in many cases, but is not required
      output.psl is where to put the output.
      Subranges of nib and .2bit files may specified using the syntax:
         /path/file.nib:seqid:start-end
      or
         /path/file.2bit:seqid:start-end
      or
         /path/file.nib:start-end
      With the second form, a sequence id of file:start-end will be used.
      options:
      -t=type     Database type.  Type is one of:
                    dna - DNA sequence
                    prot - protein sequence
                    dnax - DNA sequence translated in six frames to protein
                  The default is dna
      -q=type     Query type.  Type is one of:
                    dna - DNA sequence
                    rna - RNA sequence
                    prot - protein sequence
                    dnax - DNA sequence translated in six frames to protein
                   rnax - DNA sequence translated in three frames to protein
                The default is dna
      -prot       Synonymous with -t=prot -q=prot
      -ooc=N.ooc  Use overused tile file N.ooc.  N should correspond to 
                  the tileSize
      -tileSize=N sets the size of match that triggers an alignment.  
                  Usually between 8 and 12
                  Default is 11 for DNA and 5 for protein.
      -stepSize=N spacing between tiles. Default is tileSize.
      -oneOff=N   If set to 1 this allows one mismatch in tile and still
                  triggers an alignments.  Default is 0.
      -minMatch=N sets the number of tile matches.  Usually set from 2 to 4
                  Default is 2 for nucleotide, 1 for protein.
      -minScore=N sets minimum score.  This is the matches minus the 
                  mismatches minus some sort of gap penalty.  Default is 30
      -minIdentity=N Sets minimum sequence identity (in percent).  Default is
                  90 for nucleotide searches, 25 for protein or translated
                  protein searches.
      -maxGap=N   sets the size of maximum gap between tiles in a clump.  Usually
                  set from 0 to 3.  Default is 2. Only relevent for minMatch > 1.
      -noHead     suppress .psl header (so it's just a tab-separated file)
      -makeOoc=N.ooc Make overused tile file. Target needs to be complete genome.
      -repMatch=N sets the number of repetitions of a tile allowed before
                  it is marked as overused.  Typically this is 256 for tileSize
                  12, 1024 for tile size 11, 4096 for tile size 10.
                  Default is 1024.  Typically only comes into play with makeOoc.
                  Also affected by stepSize. When stepSize is halved repMatch is
                  doubled to compensate.
      -mask=type  Mask out repeats.  Alignments won't be started in masked region
                  but may extend through it in nucleotide searches.  Masked areas
                  are ignored entirely in protein or translated searches. Types are
                    lower - mask out lower cased sequence
                    upper - mask out upper cased sequence
                    out   - mask according to database.out RepeatMasker .out file
                   file.out - mask database according to RepeatMasker file.out
      -qMask=type Mask out repeats in query sequence.  Similar to -mask above but
                  for query rather than target sequence.
      -repeats=type Type is same as mask types above.  Repeat bases will not be
                  masked in any way, but matches in repeat areas will be reported
                  separately from matches in other areas in the psl output.
      -minRepDivergence=NN - minimum percent divergence of repeats to allow 
                  them to be unmasked.  Default is 15.  Only relevant for 
                  masking using RepeatMasker .out files.
      -dots=N     Output dot every N sequences to show program's progress
      -trimT      Trim leading poly-T
      -noTrimA    Don't trim trailing poly-A
      -trimHardA  Remove poly-A tail from qSize as well as alignments in 
                  psl output
      -fastMap    Run for fast DNA/DNA remapping - not allowing introns, 
                  requiring high %ID. Query sizes must not exceed 5000.
      -out=type   Controls output file format.  Type is one of:
                      psl - Default.  Tab separated format, no sequence
                      pslx - Tab separated format with sequence
                      axt - blastz-associated axt format
                      maf - multiz-associated maf format
                      sim4 - similar to sim4 format
                      wublast - similar to wublast format
                      blast - similar to NCBI blast format
                      blast8- NCBI blast tabular format
                      blast9 - NCBI blast tabular format with comments
    -fine       For high quality mRNAs look harder for small initial and
                  terminal exons.  Not recommended for ESTs
      -maxIntron=N  Sets maximum intron size. Default is 750000
      -extendThroughN - Allows extension of alignment through large blocks of N's  

### BLAT references

Kent WJ. 2002. BLAT—The BLAST-Like Alignment Tool. Genome Research 12:656–664. DOI: 10.1101/gr.229202.  
Kent WJ. 2012. BLAT—The BLAST-Like Alignment Tool. Version 35. Available at <https://users.soe.ucsc.edu/~kent>.  

BLATq modifications by: James Henderson, jhenderson@calacademy.org
BLATq README.md authors: Zachary R. Hanna, James Henderson
