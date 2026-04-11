#include <WebServer.h>

WebServer server(80);

const char *rootIndex=R"(
<!DOCTYPE html>
<html lang="de"><head>
<meta charset="UTF-8">
<title>ILDA Player</title>
<style>
  body { font-family:Arial,sans-serif; margin:30px; }
  select { overflow:auto; }
  #selectDir { width:30em; height:40em; }
  #selectSpeed { height:40em; }
  #selectSpeed option { text-align:right; padding-right:1em; }
  #selectRed { height:40em; }
  #selectRed option { text-align:right; padding-right:1em; }
  #selectGreen { height:40em; }
  #selectGreen option { text-align:right; padding-right:1em; }
  #selectBlue { height:40em; }
  #selectBlue option { text-align:right; padding-right:1em; }
  #selectSize { height:40em; }
  #selectSize option { text-align:right; padding-right:1em; }
  .right { float:right; padding-right:1em; }
  .top { vertical-align:top; }
</style>
<script>
function webUIinit() {
  for (n=1;n<=60;n++) {
    if (n>2) { n++; }
    if (n>40) { n+=3; }
    optionSpeed=document.createElement("option"); optionSpeed.text=n+"k"; optionSpeed.value=Math.floor(1000/n);
    if (n==20) { optionSpeed.selected=true; }
    id("selectSpeed").appendChild(optionSpeed); }
  for (n=0;n<=100;n+=5) {
    optionRed=document.createElement("option"); optionRed.text=n+"%"; optionRed.value=n;
    if (n==0) { optionRed.text="R 0%"; }
    if (n==50) { optionRed.selected=true; }
    id("selectRed").appendChild(optionRed);
    optionGreen=document.createElement("option"); optionGreen.text=n+"%"; optionGreen.value=n;
    if (n==0) { optionGreen.text="G 0%"; }
    if (n==50) { optionGreen.selected=true; }
    id("selectGreen").appendChild(optionGreen);
    optionBlue=document.createElement("option"); optionBlue.text=n+"%"; optionBlue.value=n;
    if (n==0) { optionBlue.text="B 0%"; }
    if (n==50) { optionBlue.selected=true; }
    id("selectBlue").appendChild(optionBlue);
    optionSize=document.createElement("option"); optionSize.text=n+"%"; optionSize.value=n;
    if (n==0) { optionSize.text="S 0%"; }
    if (n==100) { optionSize.selected=true; }
    id("selectSize").appendChild(optionSize); }
  red="#E09090"; green="#90E090"; yellow="#FFE460"; gray="#e0e0e0"; darkgray="#d0d0d0"; blue="#c2d5ed";
  ajaxObj=[]; requestAJAX("getDir?dir=/"); }
function requestAJAX(value) {
  ajaxObj[value]=new XMLHttpRequest; ajaxObj[value].url=value; ajaxObj[value].open("GET",value,true);
  ajaxObj[value].setRequestHeader("Content-Type","application/x-www-form-urlencoded");
  ajaxObj[value].addEventListener('load',replyAJAX); ajaxObj[value].send(); }
function replyAJAX(event) {
  if (event.target.status==200) {
    if (event.target.url.startsWith("getDir")) {
      dirArray=event.target.responseText.split("*").sort();
      position=0; while (true) {
        if (!dirArray[position]) { break; }
        if (dirArray[position].split(":")[1]=="DIR") {
          option=document.createElement("option");
          option.text=dirArray[position].split(":")[0].substring(dirArray[position].split(":")[0].lastIndexOf("/")+1);
          option.value=dirArray[position].split(":")[0];
          if (position==0) { option.value="/"; }
          id("selectDir").appendChild(option); }
        position++; }
      position=0; while (true) {
        if (!dirArray[position]) { break; }
        if (dirArray[position].split(":")[1]!="DIR") {
          option=document.createElement("option");
          option.innerHTML=dirArray[position].split(":")[0].substring(dirArray[position].split(":")[0].lastIndexOf("/")+1) + "<span class=\"right\">" + dirArray[position].split(":")[1] + "</span>";
          option.value=dirArray[position].split(":")[0];
          id("selectDir").appendChild(option); }
        position++; } } } }
function onSelectDir(event) {
  if (!event.value.toLowerCase().endsWith(".ild")) { requestAJAX("getDir?dir="+event.value); while (id("selectDir").firstChild) { id("selectDir").removeChild(id("selectDir").lastChild); } }
  else { requestAJAX("getFile?file="+event.value); } }
function onSelectSpeed(event) { requestAJAX("setSpeed?speed="+event.value); }
function onSelectRed(event) { requestAJAX("setRed?red="+event.value); }
function onSelectGreen(event) { requestAJAX("setGreen?green="+event.value); }
function onSelectBlue(event) { requestAJAX("setBlue?blue="+event.value); }
function onSelectSize(event) { requestAJAX("setSize?size="+event.value); }
function onReloadIlda(event) { if (id("selectDir").selectedOptions[0]) { name=id("selectDir").selectedOptions[0].value; if (name.toLowerCase().endsWith(".ild")) { requestAJAX("getFile?file="+name); } } }
function id(id) { return document.getElementById(id); }
</script></head>
<body onload="webUIinit();">
<select size="10" id="selectDir" onchange="onSelectDir(this);"></select>
<select size="10" id="selectSpeed" onchange="onSelectSpeed(this);"></select>
<select size="10" id="selectRed" onchange="onSelectRed(this);"></select>
<select size="10" id="selectGreen" onchange="onSelectGreen(this);"></select>
<select size="10" id="selectBlue" onchange="onSelectBlue(this);"></select>
<select size="10" id="selectSize" onchange="onSelectSize(this);"></select>
<button type="button" onclick="onReloadIlda();" class="top">Reload</button>
</body></html>)";

void handleRoot() { server.send(200,"text/html",rootIndex); }
void getDir() { server.send(200,"text/html",readDir(server.arg("dir"))); }
void getFile() { server.send(200,"text/html",""); readILDA(server.arg("file")); }
void setSpeed() { server.send(200,"text/html",""); dacSpeed=server.arg("speed").toInt(); }
void setRed() { server.send(200,"text/html",""); redBright=server.arg("red").toInt(); }
void setGreen() { server.send(200,"text/html",""); greenBright=server.arg("green").toInt(); }
void setBlue() { server.send(200,"text/html",""); blueBright=server.arg("blue").toInt(); }
void setSize() { server.send(200,"text/html",""); ildaSize=server.arg("size").toInt(); }

void initServer() {
  server.on("/",handleRoot);
  server.on("/getDir",getDir);
  server.on("/getFile",getFile);
  server.on("/setSpeed",setSpeed);
  server.on("/setRed",setRed);
  server.on("/setGreen",setGreen);
  server.on("/setBlue",setBlue);
  server.on("/setSize",setSize);
  server.begin(); }

void serverWorker() {
  static uint32_t serverTimer;
  if (millis()>=serverTimer) { serverTimer=millis()+500;
    server.handleClient(); } }
