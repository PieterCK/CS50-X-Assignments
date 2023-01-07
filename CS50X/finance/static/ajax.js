function sell(stock){
    if(stock == "")
        return;

    var ajax = new XMLHttpRequest();

    ajax.onreadystatechange = function(){
        if(ajax.readyState == 4 && ajax.status == 200) {
            document.querySelector("#stinkfo").html(ajax.responseText);
        }
    };

    ajax.open('GET', stock + '.html', true);
    ajax.send();
}