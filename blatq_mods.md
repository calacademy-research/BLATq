# blatq

Standalone BLAT v. 35 source code was modified to accept fastq files as well as fasta files.
No quality score information from the fastq file is used. This change obviates
the need to convert a fastq to fasta before running blatq with a fastq file as input.

Three C language files were minimally modified and blat's makefile run to create this version.

Assuming blat source files unzipped in a directory named blatSrc, these files are:

- blatSrc/blat/blat.c
- blatSrc/jkOwnLib/gfClientLib.c
- blatSrc/lib/fa.c

The change in blat/blat.c inserted at line 68 adds another line to the usage block (line beginning with NOTE):

    "where:\n"
    "   database and query are each either a .fa , .nib or .2bit file,\n"
    "   or a list these files one file name per line.\n"
    "   NOTE: fastq files (4 lines per record) allowed in this version.\n\n"
  
The change in jkOwnlib/gfClientLib.c inserted at line 32 permits records to start with @ as well as > in the header:

    /* Detect .fa files (where suffix is not standardized)
     * by first character being a '>'. */
    /* 30Jun14 JBH allow fastq files too, where first
     * character is an '@' (fa.c modified too) */
    else
        {
        FILE *f = mustOpen(fileName, "r");
        char c = fgetc(f);
        fclose(f);
        if (c == '>')
            gotSingle = TRUE;
        if (c == '@') /* 30Jun14 JBH allow fastq files*/
            gotSingle = TRUE;
        }
    
The change in lib/fa.c inserted at line 459 adds an else if block to handle the fastq records, as commented:

    if (line[0] == '>')
        {
        line = firstWordInLine(skipLeadingSpaces(line+1));
        if (line == NULL)
            errAbort("Expecting sequence name after '>' line %d of %s", lf->lineIx, lf->fileName);
        strncpy(name, line, sizeof(name));
        name[sizeof(name)-1] = '\0'; /* Just to make sure name is NULL terminated. */
        }
    /* 30Jun14 JBH: Allowing fastq file to be read in.
     * each rec starts with @ and is 4 lines with 2nd line the sequence */
    else if (line[0] == '@')
            {
        line = firstWordInLine(skipLeadingSpaces(line+1));
        if (line == NULL)
            errAbort("Expecting sequence name after '@' line %d of %s", lf->lineIx, lf->fileName);
        strncpy(name, line, sizeof(name));
        name[sizeof(name)-1] = '\0'; /* Just to make sure name is NULL terminated. */
    
        if (lineFileNext(lf, &line, &lineSize))
                    {
                    if (bufIx + lineSize >= faFastBufSize)
                            expandFaFastBuf(bufIx, lineSize);
                    for (i=0; i<lineSize; ++i)
                            {
                            c = line[i];
                            if (isalpha(c) || c == '-')
                                    faFastBuf[bufIx++] = c;
                            }
    
                    if (bufIx >= faFastBufSize)
                            expandFaFastBuf(bufIx, 0);
                    faFastBuf[bufIx] = 0;
                    *retDna = faFastBuf;
                    *retSize = bufIx;
                    *retName = name;
                    if (bufIx == 0)
                            {
                            warn("Invalid FastQ format: sequence size == 0 for element %s",name);
                            }
                            
                    /* throw away next 2 lines (quality indicator line and quality score line) */
                    if (lineFileNext(lf, &line, &lineSize))
                            {
                        lineFileNext(lf, &line, &lineSize);
                            }
                            
                    return TRUE;
                    }
            else
                    {
                    *retDna = NULL;
                    *retSize = 0;
                    return FALSE;
                    }
            }
    /* end 30Jun14 JBH addition for fastq */
    else
        {
        errAbort("Expecting '>' or '@' line %d of %s", lf->lineIx, lf->fileName);
        }
