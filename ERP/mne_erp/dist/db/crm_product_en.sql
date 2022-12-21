INSERT INTO mne_crm.producttree (createdate, createuser, modifydate, modifyuser, parentid, productid, treeid, treename)
  VALUES (0, 'admindb', 0, 'admindb', NULL, NULL, '0000000000000001', 'universal');

INSERT INTO mne_crm.producttree (createdate, createuser, modifydate, modifyuser, parentid, productid, treeid, treename)
  VALUES (0, 'admindb', 0, 'admindb', NULL, NULL, '0000000000000010', 'administration');

INSERT INTO mne_crm.product (createdate, createuser, description, modifydate, modifyuser, name, productid, productnumber, withworkingstep) 
  VALUES (0, 'admindb', '', 0, 'admindb', 'empty product', '0000000000000001', '', false);
  
INSERT INTO mne_crm.producttree (createdate, createuser, modifydate, modifyuser, parentid, productid, treeid, treename)
  VALUES (0, 'admindb', 0, 'admindb', '0000000000000001', '0000000000000001', '0000000000000002', 'empty product');
  
INSERT INTO mne_crm.product (createdate, createuser, description, modifydate, modifyuser, name, productid, productnumber, withworkingstep) 
  VALUES (0, 'admindb', '', 0, 'admindb', ' arrears fee', 'reminder', '1000000', false);
  
INSERT INTO mne_crm.productprice( createdate, createuser, currencyid, modifydate, modifyuser, productid, unit, unitprice, vat, unitcost)
  VALUES (0, 'admindb', 'CHF', 0, 'admindb', 'reminder', '', '50', null, '50');

INSERT INTO mne_crm.producttree (createdate, createuser, modifydate, modifyuser, parentid, productid, treeid, treename)
  VALUES (0, 'admindb', 0, 'admindb', '0000000000000010', 'reminder', '0000000000000011', ' arrears fee');
  