use strict;
use Net::Ftp;
use Log::Log4Perl qw(easy);
use File::Spec;
use File::Util;
use File::Basename;

my $user = 'u8473780';
my $pass = $ARGV[0];
my $site = 'andreas-grimme.gmxhome.de';
my $root = "$0/../..";

my $ftp = Net::FTP->new($site, Debug => 1) or LOGDIE("Cannot connect to some.host.name: $@");
$ftp->login($user, $pass) or LOGDIE("Cannot login " . $ftp->message);
$ftp->binary;
$ftp->cwd("/dicomshell") or LOGDIE("Cannot change working directory " . $ftp->message);

my $f = File::Util->new();
for (dirContents(File::Spec->catdir($root, 'website/dicomshell')))
{
	mirror($ftp, $_);
}

mirror($ftp, File::Spec->catdir($root, 'Setup\Release\DicomShell.msi'));
mirror($ftp, File::Spec->catdir($root, 'history.txt'));
$ftp->quit;

sub dirContents
{
	my ($dir)  = @_;
	return 
		map { File::Spec->catdir($dir, $_); }
		$f->list_dir($dir,'--no-fsdots');
}

sub mirror
{
	my ($ftp, $local) = @_;
	my $remote = basename($local);
	my $remoteSize = $ftp->size($remote);
	my $localSize = -s $local;
	if ($remoteSize != $localSize)
	{
		$ftp->put($local);
	}
}
