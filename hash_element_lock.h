
#ifndef HASH_ELEMENT_LOCK_H
#define HASH_ELEMENT_LOCK_H

#include <stdio.h>
#include "list_element_lock.h"

#define HASH_INDEX(_addr,_size_mask) (((_addr) >> 2) & (_size_mask))

template<class Ele, class Keytype> class hash;

template<class Ele, class Keytype> class hash {
 private:
  unsigned my_size_log;
  unsigned my_size;
  unsigned my_size_mask;
  list<Ele,Keytype> *entries;
  list<Ele,Keytype> *get_list(unsigned the_idx);

 public:
  void setup(unsigned the_size_log=5);
  void insert(Ele *e);
  Ele *lookup(Keytype the_key);
  void print(FILE *f=stdout);
  void reset();
  void cleanup();
};

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::setup(unsigned the_size_log){
  my_size_log = the_size_log;
  my_size = 1 << my_size_log;
  my_size_mask = (1 << my_size_log) - 1;
  entries = new list<Ele,Keytype>[my_size];
}

template<class Ele, class Keytype> 
list<Ele,Keytype> *
hash<Ele,Keytype>::get_list(unsigned the_idx){
  if (the_idx >= my_size){
    fprintf(stderr,"hash<Ele,Keytype>::list() public idx out of range!\n");
    exit (1);
  }
  return &entries[the_idx];
}

template<class Ele, class Keytype> 
Ele *
hash<Ele,Keytype>::lookup(Keytype the_key){
  list<Ele,Keytype> *l;


  //if(entries[HASH_INDEX(the_key,my_size_mask)]->lookup(the_key))
  l = &entries[HASH_INDEX(the_key,my_size_mask)];
  // l->lookup(the_key)->lock ->always last in list

  return l->lookup(the_key);
}  

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::print(FILE *f){
  unsigned i;

  for (i=0;i<my_size;i++){
    entries[i].print(f);
  }
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::reset(){
  unsigned i;
  for (i=0;i<my_size;i++){
    //pthread_mutex_lock(&(entries[i].list_lock));
    entries[i].cleanup();
    //pthread_mutex_unlock(&(entries[i].list_lock));
  }
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::cleanup(){
  unsigned i;
  reset();
  delete [] entries;
}

template<class Ele, class Keytype> 
void 
hash<Ele,Keytype>::insert(Ele *e){
  //pthread_mutex_lock(&(entries[HASH_INDEX(e->key(), my_size_mask)].list_lock));
  entries[HASH_INDEX(e->key(),my_size_mask)].push(e);
  //pthread_mutex_unlock(&(entries[HASH_INDEX(e->key(), my_size_mask)].list_lock));
}


#endif
