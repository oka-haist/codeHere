mutex mtx
condvar prod, cons
int nprod=0,ncons=0 //n de prod/cons abiertos
cbuffer_t cbuffer

fifoproc_open("R"){
  lock(mtx);
  while(no hay productor preparado)
    cond_wait(prodReady,mtx);
  ncons++; //un nuevo consumidor ha aparecido!
  cond_signal(consReady);
  unlock(mtx);
}

fifoproc_write(buff, len){
  //comprobacion de espacio  
  //copia de buffer de usuario a kernel
  //reserva memoria
  
  lock(mtx); //La escritura es exclusiva entre si
  
  while("no haya hueco en cbuffer AND haya algun consumidor")
    cond_wait(prod,mtx);
  
  insertar(cbuffer);
  
  cond_signal(cons); //Despierta si hay. No si no

  unlock(mtx);
  
  //liberar mem y retorno
}

fifoproc_read(buff, len){
  //comprobacion de tamanyos
  lock(mtx) //al contrario que los tipicos lectores
  	    //aqui se consume, luego se usa el Lock
  while("no hayan datos AND haya productores")
    cont_wait(cons,mtx);
  
  eliminar(cbuffer);
  
  cond_signal(prod);
  unlock(mtx);
}

fifoproc_release(){
  lock(mtx);
  cons--;
  unlock(mtx);
}