cmd /c build.bat

if ($LastExitCode -eq 0)
{
    cmd /c run.bat
}
