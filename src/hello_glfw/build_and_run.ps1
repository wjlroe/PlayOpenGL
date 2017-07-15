Measure-Command { cmd /c build.bat | Out-Default }

if ($LastExitCode -eq 0)
{
    cmd /c run.bat
}
