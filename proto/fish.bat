pushd "%~dp0"

::-------¿Í»§¶Ë---------------------
set "errorlevel="
protoc --cpp_out=./ fish.proto > nul
IF %ERRORLEVEL% NEQ 0 goto ErrorLabel

set src=..\ywfish\

if exist %src%fish.pb.h (
  del %src%fish.pb.h /F /Q > nul
)

if exist %src%fish.pb.cc (
  del %src%fish.pb.cc /F /Q > nul
)

copy fish.pb.h %src%fish.pb.h > nul
copy fish.pb.cc %src%fish.pb.cc > nul

if exist fish.pb.h (
  del fish.pb.h /F /Q > nul
)

if exist fish.pb.cc (
  del fish.pb.cc /F /Q > nul
)

if exist world_server.cs (
  del world_server.cs /F /Q
)

goto SuccessLabel

:ErrorLabel
echo ------------------------------------------
echo Error
echo ------------------------------------------
pause

:SuccessLabel

popd

