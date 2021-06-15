pipeline {
    agent {
        docker 'cuberite/docker-ci/minimal:latest'
    }
    stages {
        stage("Prepare") {
            steps {
                sh 'git submodule update --init'
            }
        }
        stage("Check") {
            parallel {
                stage("CheckBasicStyle") {
                    steps {
                        dir("src") {
                            sh 'find . -name \\*.cpp -or -name \\*.h > AllFiles.lst'
                            sh 'lua CheckBasicStyle.lua'
                            sh 'cd Bindings && lua CheckBindingsDependencies.lua'
                        }
                    }
                }
                stage("clang-tidy") {
                    steps {
                        sh './clang-tidy.sh -j 3'
                    }
                }
            }
        }
        stage("Build") {
            parallel {
                stage("gcc") {
                    environment {
                        TRAVIS_CUBERITE_BUILD_TYPE = 'Debug'
                        TRAVIS_JOB_NUMBER = "{$env.BUILD_ID}"
                    }
                    steps {
                        sh './travisbuild.sh'
                    }
                }
            }
        }
    }
}
