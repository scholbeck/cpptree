
convertColTypes = function(frame) {
  frame = df
  res = unlist(lapply(frame, class))
  res = lapply(res, FUN = function(x) {
    if ((x == "numeric") || (x == "integer")) {
      return("num")
    } else if (x == "factor") {
      return("categ")
    }
  })
  res = unlist(res)
  res = unname(res)
  return(res)
}

convertCategEncodings = function(data) {
  types = convertColTypes(data)
  categ_ix = which(types == "categ")
  encodings = lapply(categ_ix, FUN = function(j) {
    unique(as.double(data[ , j])) - 1
  })
  names(encodings) = categ_ix - 1
  return(encodings)
}

xtree = function(data, target, params) {
  coltypes = convertColTypes(data)
  categ_encodings = convertCategEncodings(data)
  
  tree = XTree$new(data, target, coltypes, categ_encodings, params)
  return(tree)
}

as_party = function(tree, data) {
  library(partykit)
  strct = tree$getTreeStructure()
  strct = data.frame(apply(strct, 2, as.character), stringsAsFactors = FALSE)
  depth = tree$depth
  
  node_list = list()
  id_cnt = nrow(strct)
  while (depth >= 0) {
    id_vec = which(nchar(strct$ID) == depth + 1)

    parent_nodes = strct[id_vec, ]
    parent_nodes = parent_nodes[parent_nodes$is_leaf == 0, ]
    leaf_nodes = strct[id_vec, ]
    leaf_nodes = leaf_nodes[leaf_nodes$is_leaf == 1, ]
    
    leafnode_list = list()
    for (i in 1:nrow(leaf_nodes)) {
      n = partynode(id = id_cnt, info = list("ID" = leaf_nodes[i, "ID"]))
      id_cnt = id_cnt - 1
      leafnode_list = append(leafnode_list, list(n))
    }
    node_list = append(node_list, leafnode_list)
    
    parentnode_list = list()
    if (nrow(parent_nodes) > 0) {
      for (i in 1:nrow(parent_nodes)) {
        parent_id = parent_nodes[i, "ID"]
        childnode_candidate_id_vec = which(nchar(strct$ID) == depth + 2)
        child_nodes = list()
        for (j in 1:length(node_list)) {
          x = node_list[[j]]
          x_id_splitstring = unlist(strsplit(as.character(x$info$ID), ""))
          x_id_pasted_prefix = paste0(x_id_splitstring[-length(x_id_splitstring)], collapse = "")
          if (parent_id == x_id_pasted_prefix) {
            child_nodes = append(child_nodes, list(x))
          } else {}
        }
      split_feature = as.integer(strct[strct$ID == parent_id, "feature"])
      split_values = as.numeric(strsplit(strct[strct$ID == parent_id, "values"], ","))
      sp = partysplit(varid = split_feature, breaks = split_values)
      n = partynode(id = id_cnt, split = sp, info = list("ID" = parent_id), kids = child_nodes)
      id_cnt = id_cnt - 1
      parentnode_list = append(parentnode_list, list(n))
      }
      node_list = append(node_list, parentnode_list)
    }
    depth = depth - 1
  }
  root = node_list[[length(node_list)]]
  party_obj = party(root, data)
  return(party_obj)
}
# party_interface = function(tree.summary, data) {
#   options(scipen = 99)
#   
#   features = colnames(data)
#   party.node.list = list()
#   depth = max(tree.summary$depth)
#   nodes = tree.summary[which(tree.summary$depth == depth), ]
#   nodes.id = as.numeric(as.character(nodes$id))
#   
#   party.nodes = lapply(nodes.id, FUN = function(node.id) {
#     
#     node.ame = nodes[nodes$id == node.id, "ame"]
#     node.sd = nodes[nodes$id == node.id, "ame.sd"]
#     node.n = nodes[nodes$id == node.id, "n.observations"]
#     node.frac = round(as.numeric(as.character(node.n)) / nrow(data) * 100, 2)
#     node.frac = paste0(node.frac, "%")
#     node.info = paste0("AME: ", node.ame, "\n", "SD: ", node.sd, "\n", "Frac.: ", node.frac)
#     party.node = partynode(id = node.id, info = node.info)
#     return(party.node)
#   })
#   
#   party.node.list = append(party.node.list, party.nodes)
#   depth = depth - 1
#   
#   while (depth > 0) {
#     
#     nodes = tree.summary[tree.summary$depth == depth, ]
#     parent.nodes = nodes[!is.na(nodes$split.feature), ]
#     leaf.nodes = nodes[is.na(nodes$split.feature), ]
#     
#     parent.party.nodes = lapply(1:nrow(parent.nodes), FUN = function(i) {
#       
#       node = parent.nodes[i, ]
#       node.id = as.numeric(as.character(node$id))
#       node.split.feature = node$split.feature
#       node.split.value = node$split.value
#       node.split.feature.index = which(features == node.split.feature)
#       
#       child.party.nodes = lapply(party.node.list, FUN = function(x) {
#         
#         x.splitstring = unlist(strsplit(as.character(x$id), ""))
#         x.pasted = paste0(x.splitstring[-length(x.splitstring)], collapse = "")
#         if (node.id == x.pasted) {
#           return(x)
#         } else {
#         }
#       })
#       
#       child.party.nodes = child.party.nodes[!unlist(lapply(child.party.nodes, is.null))]
#       # drop NULL values
#       
#       child.party.directions = lapply(child.party.nodes, FUN = function(x) {
#         
#         x = unlist(strsplit(as.character(x$id), split = ""))
#         if (x[length(x)] == "0") {
#           return("child.left")
#         } else {
#           return("child.right")
#         }
#       })
#       
#       names(child.party.nodes) = child.party.directions
#       child.party.node.left = child.party.nodes[["child.left"]]
#       child.party.node.right = child.party.nodes[["child.right"]]
#       
#       party.node = partynode(
#         node.id, split = partysplit(node.split.feature.index, node.split.value),
#         kids = list(child.party.node.left, child.party.node.right))
#       return(party.node)
#     })
#     
#     if (nrow(leaf.nodes) > 0) {
#       leaf.party.nodes = lapply(1:nrow(leaf.nodes), FUN = function(i) {
#         node = leaf.nodes[i, ]
#         node.id = as.numeric(as.character(node$id))
#         node.ame = nodes[nodes$id == node.id, "ame"]
#         node.sd = nodes[nodes$id == node.id, "ame.sd"]
#         node.n = nodes[nodes$id == node.id, "n.observations"]
#         node.frac = round(as.numeric(as.character(node.n)) / nrow(data) * 100, 2)
#         node.frac = paste0(node.frac, "%")
#         node.info = paste0("AME: ", node.ame, "\n", "SD: ", node.sd, "\n", "Frac.: ", node.frac)
#         party.node = partynode(id = node.id, info = node.info)
#         return(party.node)
#       })
#     } else {}
#     
#     party.node.list = append(party.node.list, parent.party.nodes)
#     party.node.list = append(party.node.list, leaf.party.nodes)
#     
#     depth = depth - 1
#   }
#   
#   tree.index = which(unlist(lapply(party.node.list, FUN = function(x) {
#     x$id == 1
#   })))
#   
#   party.tree = party.node.list[[tree.index]]
#   party.tree = party(party.tree, data)
#   # fitted = data.frame(
#   #   "(fitted)" = fitted_node(party.tree,
#   #                            data = data),
#   #   "(response)" = data$marginal.effect),
#   # terms = terms(marginal.effect ~ ., data = data),)
#   return(party.tree)
# }

