function show(obj, id,id2)
{
  var o=document.getElementById(id);
  var i=document.getElementById(id2);
  if( o.style.display == 'none' )
  {
    o.style.display='';
	i.style.display='none';
    obj.innerHTML='-';
  }
  else
  {
    o.style.display='none';
	i.style.display='inline';
    obj.innerHTML='+';
  }
}