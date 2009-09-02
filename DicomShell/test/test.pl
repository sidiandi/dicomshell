use strict;
use Win32::OLE;

eval
{
my $dicomFile = new Win32::OLE('DicomShell.DicomFile') or die "error";
print $dicomFile;
$dicomFile->Load('d:\devel\DicomShell\test\test.dcm', 0) or die;

};
$@ and print $@;

<STDIN>;