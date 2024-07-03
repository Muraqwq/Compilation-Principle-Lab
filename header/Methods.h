#pragma once

#include "Translate.h"
#include "memory"
#include "stdlib.h"
#include "ctype.h"
class TransMethod
{
public:
    TransMethod(Translate& trans) : translate(trans) {};
    static Symbol* Item2Symbol(char key, string val)
    {
        if (key == 11 || key == 12)
        {

            I* tmp = new I();
            if (key == 11) {
                tmp->val = val;
                tmp->type = VAR_TYPE::VAR;
            }
            else
            {
                tmp->place = stoi(val);
                tmp->type = VAR_TYPE::INT;
            }
            return tmp;
        }
        else if (key <= 14 && key >= 13)
        {
            C* tmp = new C();
            tmp->key = key;
            return tmp;
        }
        else if (key <= 19 && key >= 17)
        {
            R* tmp = new R();
            tmp->key = key;
            if (key == 17)
                tmp->val = "=";
            else if (key == 18)
                tmp->val = ">";
            else
                tmp->val = "<";
            return tmp;
        }
        else if (key == 24 || key == 25)
        {
            B* tmp = new B();
            tmp->key = key;
            return tmp;
        }
        else if (key == 'E')
        {
            return new E();
        }
        else if (key == 'S')
        {
            return new S();
        }
        else if (key == 'L')
        {
            return new L();
        }
        else if (key == 'M')
        {
            return new M();
        }
        else if (key == 'N')
        {
            return new N();
        }
        else
            return new A();
    }

    void error(string msg) {
        string error = "语法树生成过程中出现错误：" + msg;
        throw runtime_error(error);
       
    }

public:
    void IequalE2S(S* s)
    {
        I* i = (I*)s->child_nodes[0];
        E* e = (E*)s->child_nodes[1];
        if (i->type == INT) error(":=左侧必须是变量");
        i->place = e->place.val;
        if (e->type == RESERVE)
        {
            translate.emit({ ":=", e->print(), "--", i->val});
        }
        else
        {
        translate.emit({ ":=", translate.getTmp(e->place.index), "--", i->val });

        }
        //s->nextlist.push_back(translate.next_seq());
    }
    void ECE2E(E* e)
    {
        E* e1 = (E*)e->child_nodes[0];
        C* c = (C*)e->child_nodes[1];
        E* e2 = (E*)e->child_nodes[2];
        translate.new_temp(e);
        e->type = TEMP;
        string str;
        if (c->key == 13)
        {
            e->place.val = e1->place.val + e2->place.val;
            str = "+";
        }
        else
        {
            e->place.val = e1->place.val * e2->place.val;
            str = "*";
        }
        string tmp1;
        string tmp2;
        tmp1 = e1->type==RESERVE ? e1->print() : translate.getTmp(e1->place.index);
        tmp2 = e2->type==RESERVE ? e2->print() : translate.getTmp(e2->place.index);
        
        translate.emit({ str, tmp1, tmp2, translate.getTmp(e->place.index) });
    }
    // void EplusE2E(E *e)
    // {
    //     E *e1 = (E *)e->child_nodes.at(0);
    //     E *e2 = (E *)e->child_nodes.at(1);
    //     translate.new_temp(e);
    //     e->place.val = e1->place.val + e2->place.val;
    //     translate.emit({"+", translate.getTmp(e1->place.index), translate.getTmp(e2->place.index), translate.getTmp(e->place.index)});
    // };

    // void EtimesE2E(E *e)
    // {
    //     E *e1 = (E *)e->child_nodes.at(0);
    //     E *e2 = (E *)e->child_nodes.at(1);
    //     translate.new_temp(e);
    //     e->place.val = e1->place.val * e2->place.val;
    //     translate.emit({"*", translate.getTmp(e1->place.index), translate.getTmp(e2->place.index), translate.getTmp(e->place.index)});
    // };

    void ID2E(E* e)
    {
        I* i = (I*)e->child_nodes[0];
        e->type = RESERVE;
        if (i->type == INT)
        {

            e->place.index = -1;
        }
        else
        {
            if (!translate.lookup(i->val))error("变量未定义！");
            //int tmp = translate.is_assigned(i->val);
            e->place.index = -2;
            if (translate.variables.find(i->val)->second != nullptr) 
            {
                auto tmp = translate.variables.find(i->val);
                e->place = tmp->second->place;
                e->type = tmp->second->type;
            }
            else
            {
                translate.variables[i->val] = e;
                //translate.new_temp(e);
                e->place.var = i->val;
                //e->type = TEMP;
            }
            //translate.assign_tmp(i->val, e->place.index);
            //else e->place.index = tmp;
        }
        e->place.val = i->place;
        
    }

public:
    void EBME2E(E* e)
    {
        E* e1 = (E*)e->child_nodes[0];
        B* b = (B*)e->child_nodes[1];
        M* m = (M*)e->child_nodes[2];
        E* e2 = (E*)e->child_nodes[3];
        e->type = TEMP;
        if (b->key == 24)
        {
            translate.backpatch(e1->truelist, m->quad);
            e->falselist = translate.merge({ e1->falselist, e2->falselist });
            e->truelist = e2->truelist;
        }
        else
        {
            translate.backpatch(e1->falselist, m->quad);
            e->truelist = translate.merge({ e1->truelist, e2->truelist });
            e->falselist = e2->falselist;
        }
    }
    // void EorME2E(E *e)
    // {
    //     E *e1 = (E *)e->child_nodes[0];
    //     M *m = (M *)e->child_nodes[1];
    //     E *e2 = (E *)e->child_nodes[2];
    //     translate.backpatch(e1->falselist, m->quad);
    //     e->truelist = translate.merge({e1->truelist, e2->truelist});
    //     e->falselist = e2->falselist;
    // }

    // void EandME2E(E *e)
    // {
    //     E *e1 = (E *)e->child_nodes[0];
    //     M *m = (M *)e->child_nodes[1];
    //     E *e2 = (E *)e->child_nodes[2];
    //     translate.backpatch(e1->truelist, m->quad);
    //     e->falselist = translate.merge({e1->falselist, e2->falselist});
    //     e->truelist = e2->truelist;
    // }

    void notE2E(E* e)
    {
        E* e1 = (E*)e->child_nodes[0];
        e->truelist = e1->falselist;
        e->falselist = e1->truelist;

        K* k = new K();
        k->key = 26;
        e->child_nodes.insert(e->child_nodes.begin(), k);
    }

    void IrelopI2E(E* e)
    {
        I* i1 = (I*)e->child_nodes[0];
        R* re = (R*)e->child_nodes[1];
        I* i2 = (I*)e->child_nodes[2];
        e->truelist.push_back(translate.next_seq());
        e->truelist.push_back(translate.next_seq());
        translate.emit({ "j" + re->val, i1->val, i2->val, "0" });
        translate.emit({ "j", "--", "--", "0" });
    }

    void Null2M(M* m)
    {
        m->quad = translate.next_seq();
    }

public:
    void IfElse2S(S* s)
    {
        E* e = (E*)s->child_nodes[0];
        M* m1 = (M*)s->child_nodes[1];
        S* s1 = (S*)s->child_nodes[2];
        N* n = (N*)s->child_nodes[3];
        M* m2 = (M*)s->child_nodes[4];
        S* s2 = (S*)s->child_nodes[5];

        translate.backpatch(e->truelist, m1->quad);
        translate.backpatch(e->falselist, m2->quad);
        s->nextlist = translate.merge({ s1->nextlist, n->nextlist, s2->nextlist });
        // K *k_if = new K();
        // k_if->key = 5;
        // e->child_nodes.insert(0, k_if);
    }
    void Null2N(N* n)
    {
        n->nextlist.push_back(translate.next_seq());
        translate.emit({ "j", "--", "--", "0" });
    };

    void If2S(S* s)
    {
        E* e = (E*)s->child_nodes[0];
        M* m = (M*)s->child_nodes[1];
        S* s1 = (S*)s->child_nodes[2];
        translate.backpatch(e->truelist, m->quad);
        s->nextlist = translate.merge({ e->falselist, s1->nextlist });
    };

    void Whiledo2S(S* s)
    {
        M* m1 = (M*)s->child_nodes[0];
        E* e = (E*)s->child_nodes[1];
        M* m2 = (M*)s->child_nodes[2];
        S* s1 = (S*)s->child_nodes[3];
        translate.backpatch(s1->nextlist, m1->quad);
        translate.backpatch(e->truelist, m2->quad);
        s->nextlist = e->falselist;
        translate.emit({ "j", "--", "--", to_string(m1->quad) });
    }

    void BeginLend2S(S* s)
    {
        L* l = (L*)s->child_nodes[0];
        s->nextlist = l->nextlist;
    }

    void A2S(S* s)
    {
        A* a = (A*)s->child_nodes[0];
    }

    void LMStoL(L* l)
    {
        L* l1 = (L*)l->child_nodes[0];
        M* m = (M*)l->child_nodes[1];
        S* s = (S*)l->child_nodes[2];

        translate.backpatch(l1->nextlist, m->quad);
        l->nextlist = s->nextlist;
    }

    void S2L(L* l)
    {
        S* s = (S*)l->child_nodes[0];
        l->nextlist = s->nextlist;
    }

    void IntDefine(S* s)
    {
        I* i = (I*)s->child_nodes[0];
        if (i->type == INT)error("请定义一个正确的变量");
        /** [TODO]: 判断是否重定义 */
        if (!translate.lookup(i->val))
            translate.insert_int(i->val);
        else
            error("变量重定义");
    }

    void S2V(S* s) {}

private:
    Translate& translate;
};
