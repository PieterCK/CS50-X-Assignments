window.onload = setTimeout(rndm(), 5000);
function rndm(){
    console.log("it works");
    let vstr = document.querySelector('.visitor');
    vstr.innerHTML =  Math.floor(Math.random() * 10000000) + 1;
}





