@REM invokes a local install of scons (forwarding all arguments)

@python contrib\scons\scons -j 4 --site-dir=tools\site_scons %*
