def topo_order_commits():
    git_dir_path = top_git()
    branch_list = get_branch_names(git_dir_path)
    commits, roots = build_commit_graph(git_dir_path, branch_list)
    total_order = get_topo_ordered_commits(commits, roots)
    for key in branch_list:
        addback = []
        for branch in branch_list[key]:
            iterate = 0
            while branch[iterate:iterate + 11] != 'refs/heads/':
                iterate+=1
            iterate += 11
            branch = branch[iterate:]
            addback.append(branch)
        branch_list[key] = addback

    print_topo_ordered_commits_with_branch_names(commits, total_order, branch_list)

import os
import sys
import zlib

class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()

def top_git(): #return path to the .git directory
    curr_dir = "."
    while os.path.abspath(curr_dir) != "/":
        for curr_file in os.listdir(curr_dir):
            total_path = os.path.join(curr_dir, curr_file)
            if os.path.isdir(total_path):
                if curr_file == ".git":
                    final_path = os.path.abspath(curr_dir)
                    return os.path.join(final_path, ".git")
       # temp_path = Path(os.path.abspath(curr_dir))
        curr_dir = curr_dir + "../" #testing this command on the terminal goes to parent dir, Path module doesn't work since it needs to be installed
    sys.stderr.write("Not inside a Git repository")
    sys.exit(1)

def get_branch_names(all_path):
    path_to_heads = os.path.join(all_path, "refs/heads/")
    #print(path_to_heads)
    if not os.path.isdir(path_to_heads):
        sys.stderr.write("refs/heads/ directory does not exist")
        sys.exit(1)
    else:
        hashes_names = {}
        for paths, dirs, files in os.walk(path_to_heads): #walk goes through subdirs
            #need to somehow store two types of values in one structure
            if len(files) > 0:
                for each_file in files:
                    #a dictionary seems to work well
                    path_to_file = os.path.join(paths, each_file)
                    this_hash = open(path_to_file,'rb').read().decode("latin-1")
                    this_hash = this_hash.strip('\n')
              
                    if hashes_names.get(this_hash) == None:
                        hashes_names[this_hash] = []
                    hashes_names[this_hash].append(path_to_file)
        return hashes_names

def get_parents(path, comm_hash):
    node_parents = set()
    #No need to unpack .pack files
    path_to_commit = path + comm_hash[:2] + "/" + comm_hash[2:]
    if os.path.isfile(path_to_commit):
        current_object = open(path_to_commit, 'rb').read()

        current_object = zlib.decompress(current_object).decode("latin-1")
        
        current_object = current_object.replace(" ","\n") #turn it into a list
        text_list = current_object.split("\n")
        flag = 0
        for entry in text_list:
            if flag == 1:
                node_parents.add(entry)
                flag = 0 #don't break in case there is a merge commit
            if entry == 'author':
                break
            if entry == 'parent':
                flag = 1
    #error report?
    node_parents = sorted(node_parents)
    return node_parents
        

def build_commit_graph(git_dir, local_branch_heads): #make sure git_dir is path to .git
    path_to_objects = os.path.join(git_dir, "objects/")
    commit_nodes = {}
    root_hashes = []
    visited = set()
    stack = []
    for key,val in local_branch_heads.items():
        temp = [key,val]
        stack.append(temp)
    key_list = []
    for i in stack:
        key_list.append(i[0])
    #print(key_list)
    #print(local_branch_heads)
    #print(stack)
    #commit_node = CommitNode('a6431415df76cf377592a64474d4c564c80bc58f')
    #commit_node.parents = get_parents(path_to_objects, 'a6431415df76cf377592a64474d4c564c80bc58f')
    while stack:
        #print(stack)
        commit_hash = stack.pop()
        #print(commit_hash)
        commit_hash = commit_hash[0]
        #print(commit_hash)
        if commit_hash in visited:
            continue
        visited.add(commit_hash)
        #print(visited)
        #print(commit_hash)
        if commit_hash not in commit_nodes:
            commit_nodes[commit_hash] = CommitNode(commit_hash)
        commit_node = commit_nodes[commit_hash] #get the commit_nodes properly and store them
        commit_node.parents = get_parents(path_to_objects, commit_hash)
        if not commit_node.parents:
            root_hashes.append(commit_hash)
        for p in commit_node.parents:
            if p not in visited:
                stack.append([p])
            if p not in commit_nodes:
                commit_nodes[p] = CommitNode(p)
            commit_nodes[p].children.add(commit_hash)
    root_hashes = sorted(root_hashes)
    return commit_nodes, root_hashes

def get_topo_ordered_commits (commit_nodes, root_hashes):
    order = [] 
    visited = set()
    temp_stack = [] 
    stack = sorted(root_hashes)
    while stack:
        v = stack.pop()
        if v in visited:
            continue
        visited.add(v)
        if temp_stack:
            current_top = temp_stack[-1]
        while temp_stack and v not in commit_nodes[current_top].children:
            g = temp_stack.pop()
            order.append(g)
            current_top = temp_stack[-1]
        temp_stack.append(v)
        for c in sorted(commit_nodes[v].children):
            if c in visited:
                continue
            stack.append(c)
    while temp_stack:
        p = temp_stack.pop()
        order.append(p)
    return order

def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
    jumped = False
    for i in range(len(topo_ordered_commits)):
        commit_hash = topo_ordered_commits[i]
        if jumped:
            jumped = False
            sticky_hash = ' '.join(commit_nodes[commit_hash].children)
            print(f'={sticky_hash}')
        branches = sorted(head_to_branches[commit_hash]) if commit_hash in head_to_branches else []
        print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
        if i+1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
            jumped = True
            sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
            print(f'{sticky_hash}=\n')

if __name__ == "__main__":
    topo_order_commits()
