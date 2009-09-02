use strict;
use Win32::OLE;
use File::Spec;
use Log::Log4perl qw(:easy);

Log::Log4perl->easy_init($DEBUG);
$Win32::OLE::Warn = 3;

my $d = new Win32::OLE('DicomShell.DicomFile') or die;
INFO($d);
INFO(keys %{ $d } );

$d->Open('D:\devel\DicomShell\test-data\MR-MONO2-16-knee');

print $d->{PatientsName};
